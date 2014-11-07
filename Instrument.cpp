#include <cmath>
#include <cstdlib>
#include <vector>
#include "Instrument.h"

Instrument::Instrument(const std::vector<double>& harmonics, double attackTime, double decayTime, double sustainLevel, double releaseTime)
{
	harmonics_ = harmonics;
	attackTime_ = attackTime;
	decayTime_ = decayTime;
	sustainLevel_ = sustainLevel;
	releaseTime_ = releaseTime;
	return;
}

double Instrument::getReleaseTime() const
{
	return releaseTime_;
}

double Instrument::getAttackWaveValue(double time) const
{
	return (attackTime_ == 0 ? 1 : time / attackTime_);
}

double Instrument::getDecayWaveValue(double time) const
{
	return (1 - (1 - sustainLevel_) * time / decayTime_);
}

double Instrument::getReleaseWaveValue(double time) const
{
	if (time < releaseTime_)
		return (sustainLevel_ * (releaseTime_ - time) / releaseTime_);
	return 0;
}

double Instrument::ADSR(double time, double duration) const // time in seconds
{
	if (time > duration)
		return getReleaseWaveValue(time - duration);
	if (time <= attackTime_)
		return getAttackWaveValue(time);
	if (time <= attackTime_ + decayTime_)
		return getDecayWaveValue(time - attackTime_);
	return sustainLevel_;
}

double Instrument::getWaveValue(double frequency, double volume, double time, double duration) const // time in seconds/SAMPLE_RATE; duration in seconds
{
	double waveValue = sin(frequency * time);
	for (int i = 0; i < harmonics_.size(); ++i)
		waveValue += (harmonics_[i] * sin((i + 2) * frequency * time));
	double timeInSeconds = time / (double) SAMPLE_RATE;
	return (ADSR(timeInSeconds, duration) * volume * waveValue);
}
