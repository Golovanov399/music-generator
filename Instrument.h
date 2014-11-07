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

	double getReleaseTime() const;
	double getAttackWaveValue(double time) const;
	double getDecayWaveValue(double time) const;
	double getReleaseWaveValue(double time) const;
	double ADSR(double time, double duration) const;
	double getWaveValue(double frequency, double volume, double time, double duration) const;
};

const Instrument noInstrument(std::vector<double>(0), 0.0, 0.0, 1.0, 0.0);

const double sawHarmonics[8] = {1.0/2.0, 1.0/3.0, 1.0/4.0, 1.0/5.0, 1.0/6.0, 1.0/7.0, 1.0/8.0, 1.0/9.0};
const Instrument sawWave(std::vector<double>(sawHarmonics, sawHarmonics + 8), 0.0, 0.0, 1.0, 0.0);

const double squareHarmonics[8] = {0, 1.0/3.0, 0, 1.0/5.0, 0, 1.0/7.0, 0, 1.0/9.0};
const Instrument squareWave(std::vector<double>(squareHarmonics, squareHarmonics + 8), 0.0, 0.0, 1.0, 0.0);

#endif
