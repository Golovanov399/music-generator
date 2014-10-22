#ifndef __Instrument__

#define __Instrument__

#include <cstdlib>
#include <vector>

const int SAMPLE_RATE = 44100;

class Instrument
{
	std::vector<double> harmonics_;
	double attackTime_;
	double decayTime_;
	double sustainLevel_;
	double releaseTime_;
public:
	Instrument(const std::vector<double>& harmonics, double attackTime, double decayTime, double sustainLevel, double releaseTime);
	double ADSR(double time, double duration) const;
	double getWaveValue(double frequency, double volume, double time, double duration) const;
};

#endif
