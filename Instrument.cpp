#include <cassert>
#include <cmath>
#include <cstdlib>
#include <memory>
#include <vector>
#include "Instrument.h"
#include "CValues.h"

Harmonics::Harmonics() :
	normalizeIndex(1.0)
{ }

Harmonics::Harmonics(const std::map<double, double>& someHarmonics, double index) :
	harmonics(someHarmonics),
	normalizeIndex(index)
{ }

Instrument::Instrument() :
	modulationIndex0_(0),
	modulationRatio_(0),
	attackTime_(Instrument::minAttack()),
	decayTime_(0),
	sustainLevel_(1.0),
	releaseTime_(Instrument::minRelease())
{
	harmonics_.harmonics[1.0] = 1.0;
}

Instrument::Instrument(const Harmonics& harmonics) :
	harmonics_(harmonics),
	modulationIndex0_(0),
	modulationRatio_(0),
	attackTime_(Instrument::minAttack()),
	decayTime_(0),
	sustainLevel_(1.0),
	releaseTime_(Instrument::minRelease())
{ }


Instrument::Instrument(double attackTime, double decayTime, double sustainLevel, double releaseTime) :
	modulationIndex0_(0),
	modulationRatio_(0),
	attackTime_(attackTime),
	decayTime_(decayTime),
	sustainLevel_(sustainLevel),
	releaseTime_(releaseTime)
{
	harmonics_.harmonics[1.0] = 1.0;
}

Instrument::Instrument(const Harmonics& harmonics, double attackTime, double decayTime, double sustainLevel, double releaseTime) :
	harmonics_(harmonics),
	modulationIndex0_(0),
	modulationRatio_(0),
	attackTime_(attackTime),
	decayTime_(decayTime),
	sustainLevel_(sustainLevel),
	releaseTime_(releaseTime)
{ }

Instrument::Instrument( const Harmonics& harmonics,
			double modulationIndex,
			double modulationRatio,
			double attackTime,
			double decayTime,
			double sustainLevel,
			double releaseTime) :
	harmonics_(harmonics),
	modulationIndex0_(modulationIndex),
	modulationRatio_(modulationRatio),
	attackTime_(attackTime),
	decayTime_(decayTime),
	sustainLevel_(sustainLevel),
	releaseTime_(releaseTime)
{ }

const int Instrument::maxHarmonicNumber()
{
	return 10;
}

const double Instrument::minAttack()
{
	return 0.001;
}

const double Instrument::minRelease()
{
	return 0.001;
}

const double Instrument::minReleaseLevel()
{
	return 0.01;
}

const double Instrument::minModulationIndex()
{
	return 0.01;
}

std::map<double, double> Instrument::getHarmonics() const
{
	return harmonics_.harmonics;
}

double Instrument::getModulationIndex0() const
{
	return modulationIndex0_;
}

double Instrument::getModulationRatio() const	
{
	return modulationRatio_;
}

double Instrument::getAttackTime() const
{
	return attackTime_;
}

double Instrument::getDecayTime() const
{
	return decayTime_;
}

double Instrument::getSustainLevel() const
{
	return sustainLevel_;
}

double Instrument::getReleaseTime() const
{
	return releaseTime_;
}

const int Instrument::numberOfWaves()
{
	return 3;
}

const Harmonics Instrument::sawHarmonics(int numberOfHarmonics)
{
	std::map<double, double> harmonics;
	for (static int i = 1; i <= numberOfHarmonics; ++i)
		harmonics[i] = 1.0 / static_cast<double>(i);
	return Harmonics(harmonics, PI / 2.0);
}

const Harmonics Instrument::squareHarmonics(int numberOfHarmonics)
{
	std::map<double, double> harmonics;
	for (int i = 1; i <= numberOfHarmonics; ++i)
		if (i % 2 == 1)
			harmonics[i] = 1.0 / static_cast<double>(i);
	return Harmonics(harmonics, PI / 4.0);
}

const Harmonics Instrument::triangleHarmonics(int numberOfHarmonics)
{
	std::map<double, double> harmonics;
	for (int i = 1; i <= numberOfHarmonics; ++i)
	{
		if (i % 4 == 1)
			harmonics[i] = 1.0 / static_cast<double>(i) / static_cast<double>(i);
		if (i % 4 == 3)
			harmonics[i] = -1.0 / static_cast<double>(i) / static_cast<double>(i);
	}
	return Harmonics(harmonics, PI * PI / 8.0);
}

const Instrument Instrument::randomWave()
{
	srand(time(NULL));
	int mode = rand() % Instrument::numberOfWaves();	

	std::map<int, Harmonics> waveHarmonics;	
	waveHarmonics[0] = sawHarmonics(Instrument::maxHarmonicNumber());
	waveHarmonics[1] = squareHarmonics(Instrument::maxHarmonicNumber());
	waveHarmonics[2] = triangleHarmonics(Instrument::maxHarmonicNumber());

	return Instrument(waveHarmonics[mode]);
}

double Instrument::getRealDuration(double duration) const
{
	if (decayTime_ == 0 && sustainLevel_ == 0)
		return attackTime_ + releaseTime_;
	return duration + releaseTime_;
}

double Instrument::getAttackVolume(double time) const
{
	return (attackTime_ == 0 ? 1 : time / attackTime_);
}

double Instrument::getDecayVolume(double time) const
{
	return 1.0 - (1.0 - sustainLevel_) * time / decayTime_;
}

double Instrument::getReleaseVolume(double lastLevel, double time) const
{
	if (time < releaseTime_)
		return lastLevel * (releaseTime_ - time) / releaseTime_;
	return 0;
}

double Instrument::ADSR(double frequency, double time, double duration) const // time in seconds
{
	if (decayTime_ == 0 && sustainLevel_ == 0)
	{
		if (time > attackTime_)
			return getReleaseVolume(1.0, time - attackTime_);
		return getAttackVolume(time);
	}
	if (time > duration)
		return getReleaseVolume(ADSR(frequency, duration, duration), time - duration);
	if (time <= attackTime_)
		return getAttackVolume(time);
	if (time <= attackTime_ + decayTime_)
		return getDecayVolume(time - attackTime_);
	return sustainLevel_;
}

double Instrument::getModuloIndexValue(double time, double duration) const
{
	return modulationIndex0_ * pow(Instrument::minModulationIndex(), time / getRealDuration(duration));
}

double Instrument::getWaveValue(double frequency, // frequency contains 2pi/SAMPLE_RATE
				double phase,
				double volume,
				double time, // time in seconds/SAMPLE_RATE; 
				double duration) const  // duration in seconds
{
	double timeInSeconds = time / static_cast<double>(SAMPLE_RATE);
	double waveValue = 0;
	for (auto harmonic : harmonics_.harmonics)
	{
		//if (harmonic.first * frequency * SAMPLE_RATE / TWO_PI <= 4000.0)
		waveValue += (harmonic.second * sin(
		     	      harmonic.first * frequency * time +
		     	      getModuloIndexValue(timeInSeconds, duration) * sin(modulationRatio_ * frequency * time) +
		     	      phase));
	}
	waveValue /= harmonics_.normalizeIndex;
	return ADSR(frequency, timeInSeconds, duration) * volume * waveValue;
}

Piano::Piano()
     : Instrument(Piano::pianoHarmonics(), Piano::pianoAttack(), 0, 0, Piano::pianoRelease())
{ }

Piano::Piano(const Harmonics& harmonics)
     : Instrument(harmonics, Piano::pianoAttack(), 0, 0, Piano::pianoRelease())
{ }

double Piano::getAttackVolume(double time) const
{
	if (time < attackTime_ / 2.0)
		return (attackTime_ == 0 ? 1 : std::min(1.0, time / (attackTime_ / 2.0)));
	return 1.0;
}

double Piano::getReleaseVolume(double lastLevel, double time) const
{
	return lastLevel * pow(Instrument::minReleaseLevel(), (time / releaseTime_ ));
}

const Harmonics Piano::pianoHarmonics()
{
	std::map<double, double> harmonics = {{1.0, 1.0}, {0.5, 1.0/3.0}, {1.5, 0.25}, {2.0, 0.5}, {4.0, 0.25}, {6.0, 0.25},
					       {8.0, 0.125}, {10.0, 0.125}};
	return Harmonics(harmonics, 1.0);
}

const double Piano::pianoAttack()
{
	return 0.01;
}

const double Piano::pianoRelease()
{
	return 0.5;
}

windInstrument::windInstrument(const Harmonics& harmonics, double attackTime, double decayTime, double sustainLevel, double releaseTime)
     : Instrument(harmonics, attackTime, decayTime, sustainLevel, releaseTime)
{ }

const int windInstrument::numberOfFlutes()
{
	return 3;
}

const windInstrument windInstrument::Flute(int mode)
{
	assert (mode >= 0 && mode < windInstrument::numberOfFlutes());
	
	std::map<int, Harmonics> fluteHarmonics;	
	fluteHarmonics[0] = Instrument::triangleHarmonics(Instrument::maxHarmonicNumber());
	fluteHarmonics[1] = Harmonics({{1.0, 1.0}, {3.0, -0.25}}, 1.25);
	fluteHarmonics[2] = Harmonics({{1.0, 1.0}, {2.0, 0.28}, {3.0, 4.25}, {4.0, 2.0/15.0}, {5.0, 0.3}, {6.0, 2.0/33.0}, {7.0, 0.185}, {8.0, 0.0035}}, 5.26469);

	return windInstrument(fluteHarmonics[mode], 0.2, 0.01, 5.0/6.0, 0.2);
}

const windInstrument windInstrument::randomFlute()
{
	srand(time(NULL));
	return Flute(rand() % windInstrument::numberOfFlutes());
}

const int windInstrument::numberOfOrgans()
{
	return 4;
}

const windInstrument windInstrument::Organ(int mode)
{
	assert (mode >= 0 && mode < windInstrument::numberOfOrgans());

	std::map<int, Harmonics> organHarmonics;	
	organHarmonics[0] = Harmonics({{1.0, 1.0}, {0.25, 0.2}, {0.5, 0.4}, {1.5, 0.1}, {2.0, 0.2}, {4.0, 0.1}, {6.0, 0.05}}, 1.44591);
	organHarmonics[1] = Harmonics({{1.0, 1.0}, {0.25, 0.1}, {0.5, 0.2}, {1.5, 0.1}, {2.0, 0.2}, {4.0, 0.1}, {6.0, 0.05}}, 1.2817);
	organHarmonics[2] = Harmonics({{1.0, 1.0}, {0.25, 0.3}, {0.5, 0.1}, {0.75, 0.1}, {1.5, 0.05}, {2.0, 0.1}, {4.0, 0.05}, {6.0, 0.01}}, 1.31344);
	organHarmonics[3] = Harmonics({{1.0, 1.0}, {0.25, 0.2}, {0.5, 0.1}, {0.75, 0.05}, {1.5, 0.05}, {2.0, 0.1}, {4.0, 0.05}, {6.0, 0.01}}, 1.22893);

	return windInstrument(organHarmonics[mode], 0.1, 0, 1.0, 0.1);
}

const windInstrument windInstrument::randomOrgan()
{
	srand(time(NULL));
	return Organ(rand() % windInstrument::numberOfOrgans());
}

double windInstrument::getAttackVolume(double time) const
{
	return sin(PI * time / (2.0 * attackTime_));
}

double windInstrument::getReleaseVolume(double lastLevel, double time) const
{
	return lastLevel * sin(PI / 2.0 * (1.0 + time / releaseTime_));
}

std::unique_ptr<Instrument> randomInstrument()
{
	srand(time(NULL));
	int mode = rand() % NUMBER_OF_INSTRUMENTS;
	std::unique_ptr<Instrument> instrument;
	if (mode == 0)
		instrument = std::unique_ptr<Instrument>(new Instrument(Instrument::randomWave()));
	if (mode == 1)
		instrument = std::unique_ptr<Instrument>(new Piano());
	if (mode == 2)
		instrument = std::unique_ptr<Instrument>(new windInstrument(windInstrument::randomFlute()));
	if (mode == 3)
		instrument = std::unique_ptr<Instrument>(new windInstrument(windInstrument::randomOrgan()));
	return instrument;
}

Bell::Bell(double releaseTime, double modulationIndex, double modulationRatio)
     : Instrument(Instrument::triangleHarmonics(11), modulationIndex, modulationRatio, Instrument::minAttack(), 0, 0, releaseTime)
{ }

double Bell::getReleaseVolume(double lastLevel, double time) const
{
	return lastLevel * pow(Instrument::minReleaseLevel(), (time / releaseTime_));
}
