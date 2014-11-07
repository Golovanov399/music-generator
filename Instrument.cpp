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

double Instrument::ADSR(double time, double duration) const // time in seconds
{
	if (time <= attackTime_)
		return (attackTime_ == 0 ? 1 : time / attackTime_);
	if (time <= attackTime_ + decayTime_)
		return (1 - (1 - sustainLevel_) * (time - attackTime_) / decayTime_);
	if (time <= duration)
		return sustainLevel_;
	if (time <= duration + releaseTime_)
		return (sustainLevel_ * (duration + releaseTime_ - time) / releaseTime_);
	return 0;
}

double Instrument::getWaveValue(double frequency, double volume, double time, double duration) const // time in seconds/SAMPLE_RATE; duration in seconds
{
	double waveValue = sin(frequency * time);
	for (int i = 0; i < harmonics_.size(); ++i)
		waveValue += (harmonics_[i] * sin((i + 2) * frequency * time));
	double timeInSeconds = time / (double) SAMPLE_RATE;
	return (ADSR(timeInSeconds, duration) * volume * waveValue);
}
