#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "Instrument.h"
#include "CValues.h"

Instrument::Instrument() :
	modulationIndex0_(0),
	modulationRatio_(0),
	attackTime_(MIN_ATTACK),
	decayTime_(0),
	sustainLevel_(1.0),
	releaseTime_(MIN_RELEASE)
{
	harmonics_[1.0] = 1.0;
}

Instrument::Instrument(const std::map<double, double>& harmonics) :
	harmonics_(harmonics),
	modulationIndex0_(0),
	modulationRatio_(0),
	attackTime_(MIN_ATTACK),
	decayTime_(0),
	sustainLevel_(1.0),
	releaseTime_(MIN_RELEASE)
{ }


Instrument::Instrument(double attackTime, double decayTime, double sustainLevel, double releaseTime) :
	modulationIndex0_(0),
	modulationRatio_(0),
	attackTime_(attackTime),
	decayTime_(decayTime),
	sustainLevel_(sustainLevel),
	releaseTime_(releaseTime)
{
	harmonics_[1.0] = 1.0;
}

Instrument::Instrument(const std::map<double, double>& harmonics, double attackTime, double decayTime, double sustainLevel, double releaseTime) :
	harmonics_(harmonics),
	modulationIndex0_(0),
	modulationRatio_(0),
	attackTime_(attackTime),
	decayTime_(decayTime),
	sustainLevel_(sustainLevel),
	releaseTime_(releaseTime)
{ }

Instrument::Instrument( const std::map<double, double>& harmonics,
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

std::map<double, double> Instrument::getHarmonics() const
{
	return harmonics_;
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

const std::map<double, double> Instrument::sawHarmonics(int numberOfHarmonics)
{
	std::map<double, double> harmonics;
	for (static int i = 1; i <= numberOfHarmonics; ++i)
		harmonics[i] = 1.0 / static_cast<double>(i);
	return harmonics;
}

const std::map<double, double> Instrument::squareHarmonics(int numberOfHarmonics)
{
	std::map<double, double> harmonics;
	for (int i = 1; i <= numberOfHarmonics; ++i)
		if (i % 2 == 1)
			harmonics[i] = 1.0 / static_cast<double>(i);
	return harmonics;
}

const std::map<double, double> Instrument::triangleHarmonics(int numberOfHarmonics)
{
	std::map<double, double> harmonics;
	for (int i = 1; i <= numberOfHarmonics; ++i)
	{
		if (i % 4 == 1)
			harmonics[i] = 1.0 / static_cast<double>(i) / static_cast<double>(i);
		if (i % 4 == 3)
			harmonics[i] = -1.0 / static_cast<double>(i) / static_cast<double>(i);
	}
	return harmonics;
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
		return getReleaseVolume(sustainLevel_, time - duration);
	if (time <= attackTime_)
		return getAttackVolume(time);
	if (time <= attackTime_ + decayTime_)
		return getDecayVolume(time - attackTime_);
	return sustainLevel_;
}

double Instrument::getModuloIndexValue(double time, double duration) const
{
	return modulationIndex0_ * pow(MIN_MODULO_INDEX, time / getRealDuration(duration));
}

double Instrument::getWaveValue(double frequency,
				double phase,
				double volume,
				double time, // time in seconds/SAMPLE_RATE; 
				double duration) const  //duration in seconds
{
	double timeInSeconds = time / static_cast<double>(SAMPLE_RATE);
	double waveValue = 0;
	for (auto harmonic : harmonics_)
		waveValue += (harmonic.second * sin(
		     	      harmonic.first * frequency * time +
		     	      getModuloIndexValue(timeInSeconds, duration) * sin(modulationRatio_ * frequency * time) +
		     	      phase));
	//fprintf(stderr, "%f\n", ADSR(frequency, timeInSeconds, duration));
	return ADSR(frequency, timeInSeconds, duration) * volume * waveValue;
}

Piano::Piano(double attackTime, double releaseTime)
     : Instrument(Piano::pianoHarmonics(), attackTime, 0, 0, releaseTime)
{ }

Piano::Piano(const std::map<double, double>& harmonics, double attackTime, double releaseTime)
     : Instrument(harmonics, attackTime, 0, 0, releaseTime)
{ }

double Piano::getAttackVolume(double time) const
{
	return (attackTime_ == 0 ? 1 : std::min(1.0, time / (attackTime_ / 2.0)));
}

double Piano::getReleaseVolume(double lastLevel, double time) const
{
	return lastLevel * pow(MIN_RELEASE_LEVEL, (time / releaseTime_ ));
}

const std::map<double, double> Piano::pianoHarmonics()
{
	//std::map<double, double> harmonics = {{1.0, 1.0}, {2.0, -1.0}, {3.0, -0.1}, {4.0, 0.5}, {5.0, 0.1}, {6.0, -0.1}};
	//std::map<double, double> harmonics = {{1.0, 1.0}, {0.5, 1.0/6.0}, {1.5, 0.0794}, {2.0, 0.3251}, {4.0, 0.25}, {6.0, 1.0/6.0},
	//				       {8.0, 0.125}, {10.0, 0.1}};
	//std::map<double, double> harmonics = {{1.0, 1.0}, {0.5, 1.0/6.0}, {1.5, 0.0794}, {2.0, 0.8251}, {4.0, 0.25}, {6.0, 1.0/6.0},
	//				       {8.0, 0.125}, {10.0, 0.1}};
	//std::map<double, double> harmonics = {{1.0, 1.0}, {0.5, 0.5}, {1.5, 0.375}, {2.0, 0.75}, {4.0, 0.375}, {6.0, 0.375},
	//				       {8.0, 0.1875}, {10.0, 0.1875}};
	std::map<double, double> harmonics = {{1.0, 1.0}, {0.5, 1.0/3.0}, {1.5, 0.25}, {2.0, 0.5}, {4.0, 0.25}, {6.0, 0.25},
					       {8.0, 0.125}, {10.0, 0.125}};
	return harmonics;
}

windInstrument::windInstrument(const std::map<double, double>& harmonics, double attackTime, double decayTime, double sustainLevel, double releaseTime)
     : Instrument(harmonics, attackTime, decayTime, sustainLevel, releaseTime)
{ }

const std::map<double, double> windInstrument::fluteHarmonics()
{
	std::map<double, double> harmonics = {{1.0, 2.54}, {2.0, 0.245}, {3.0, 0.009}, {4.0, 0.00001}};
	return harmonics;
}

const std::map<double, double> windInstrument::clarinetHarmonics()
{
	std::map<double, double> harmonics = {{1.0, 7.9}, {2.0, 0.076}, {3.0, 0.99}, {4.0, 0.032}, {5.0, 0.013}, {6.0, 0.004},
					      {7.0, 0.0052}, {8.0, 0.001}, {9.0, 0.0008}, {10.0, 0.0006}, {11.0, 0.0002}};
	return harmonics;
}

const std::map<double, double> windInstrument::trumpetHarmonics()
{
	std::map<double, double> harmonics = {{1.0, 0.17}, {3.0, 0.63}, {5.0, 0.57}, {7.0, 0.98}, {9.0, 0.56}, {11.0, 0.38},
					      {12.0, 0.19}, {13.0, 0.05}, {14.0, 0.03}, {15.0, 0.02}, {16.0, 0.01}};
	return harmonics;
}

double windInstrument::getAttackVolume(double time) const
{
	return sin(PI * time / (2.0 * attackTime_));
}

double windInstrument::getDecayVolume(double time) const
{
	return sustainLevel_ + (1.0 - sustainLevel_) * sin(PI / 2.0 * (1.0 + time / decayTime_));
}

Bell::Bell(double releaseTime, double modulationIndex, double modulationRatio)
     : Instrument(Instrument::triangleHarmonics(11), modulationIndex, modulationRatio, MIN_ATTACK, 0, 0, releaseTime)
{ }

double Bell::getReleaseVolume(double lastLevel, double time) const
{
	return lastLevel * pow(MIN_RELEASE_LEVEL, (time / releaseTime_));
}
