#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "Instrument.h"

Instrument::Instrument() :
	modulationIndex0_(0),
	modulationFrequency_(0),
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
	modulationFrequency_(0),
	attackTime_(MIN_ATTACK),
	decayTime_(0),
	sustainLevel_(1.0),
	releaseTime_(MIN_RELEASE)
{ }


Instrument::Instrument(double attackTime, double decayTime, double sustainLevel, double releaseTime) :
	modulationIndex0_(0),
	modulationFrequency_(0),
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
	modulationFrequency_(0),
	attackTime_(attackTime),
	decayTime_(decayTime),
	sustainLevel_(sustainLevel),
	releaseTime_(releaseTime)
{ }

Instrument::Instrument( const std::map<double, double>& harmonics,
			double modulationIndex,
			double modulationFrequency,
			double attackTime,
			double decayTime,
			double sustainLevel,
			double releaseTime) :
	harmonics_(harmonics),
	modulationIndex0_(modulationIndex),
	modulationFrequency_(modulationFrequency),
	attackTime_(attackTime),
	decayTime_(decayTime),
	sustainLevel_(sustainLevel),
	releaseTime_(releaseTime)
{ }

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

double Instrument::getReleaseVolume(double time) const
{
	if (time < releaseTime_)
		return sustainLevel_ * (releaseTime_ - time) / releaseTime_;
	return 0;
}

double Instrument::ADSR(double frequency, double time, double duration) const // time in seconds
{
	if (time > duration)
		return getReleaseVolume(time - duration);
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
		     	      getModuloIndexValue(timeInSeconds, duration) * sin(modulationFrequency_ * frequency * time) +
		     	      phase));
	return ADSR(frequency, timeInSeconds, duration) * volume * waveValue;
}

Piano::Piano(double attackTime, double decayTime, double sustainLevel, double releaseTime)
     : Instrument(Piano::pianoHarmonics(), attackTime, decayTime, sustainLevel, releaseTime)
{ }

Piano::Piano(const std::map<double, double>& harmonics, double attackTime, double decayTime, double sustainLevel, double releaseTime)
     : Instrument(harmonics, attackTime, decayTime, sustainLevel, releaseTime)
{ }

double Piano::getDecayVolume(double time) const
{
	return pow(sustainLevel_, (time / decayTime_));
}

double Piano::getReleaseVolume(double time) const
{
	return sustainLevel_ * pow(MIN_RELEASE_LEVEL, (time / releaseTime_));
}

const std::map<double, double> Piano::pianoHarmonics()
{
	//std::map<double, double> harmonics = {{1.0, 1.0}, {2.0, -1.0}, {3.0, 0.1}, {4.0, -0.5}, {5.0, -0.1}, {6.0, -0.1}};
	//std::map<double, double> harmonics = {{1.0, 1.0}, {3.0, -0.25}, {5.0, 0.0625}, {7.0, -0.015625}};
	//std::map<double, double> harmonics = {{1.0, 1.0}, {3.0, -1.0/27.0}, {5.0, 0.008},
	//				      {7.0, -1.0/343.0}, {9.0, 1.0/729.0}, {11.0, -1.0/1331.0}};
	std::map<double, double> harmonics = Instrument::triangleHarmonics(15);
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
	return sin(M_PI * time / (2.0 * attackTime_));
}

double windInstrument::getDecayVolume(double time) const
{
	return sustainLevel_ + (1.0 - sustainLevel_) * sin(M_PI / 2.0 * (1.0 + time / decayTime_));
}

Bell::Bell(double decayTime, double modulationIndex, double modulationFrequency)
     : Instrument(Instrument::triangleHarmonics(11), 0, decayTime, 0, 0)
{ }

double Bell::getRealDuration(double duration) const
{
	return decayTime_;
}

double Bell::getDecayVolume(double time) const
{
	return pow(MIN_RELEASE_LEVEL, (time / decayTime_));
}
