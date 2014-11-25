#pragma once

#include <cstdlib>
#include <map>
#include <vector>

const int SAMPLE_RATE = 44100;
const double MIN_ATTACK = 0.001;
const double MIN_RELEASE = 0.001;
const double MIN_RELEASE_LEVEL = 0.01;
const double MIN_MODULO_INDEX = 0.01;

class Instrument
{
protected:
	std::map<double, double> harmonics_;
	double modulationIndex0_;
	double modulationFrequency_;	
	double attackTime_;
	double decayTime_;
	double sustainLevel_;
	double releaseTime_;
public:
	Instrument();
	Instrument(const std::map<double, double>& harmonics);
	Instrument(double attackTime, double decayTime, double sustainLevel, double releaseTime);
	Instrument(const std::map<double, double>& harmonics, double attackTime, double decayTime, double sustainLevel, double releaseTime);
	Instrument(const std::map<double, double>& harmonics, double modulationIndex, double modulationFrequency, double attackTime, double decayTime, double sustainLevel, double releaseTime);

	static const std::map<double, double> sawHarmonics(int numberOfHarmonics);
	static const std::map<double, double> squareHarmonics(int numberOfHarmonics);
	static const std::map<double, double> triangleHarmonics(int numberOfHarmonics);

	virtual double getRealDuration(double duration) const;
	virtual double getAttackVolume(double time) const;
	virtual double getDecayVolume(double time) const;
	virtual double getReleaseVolume(double time) const;
	
	double ADSR(double frequency, double time, double duration) const;	
	double getModuloIndexValue(double time, double duration) const;

	double getWaveValue(double frequency, double phase, double volume, double time, double duration) const;
};

class Piano: public Instrument
{
public:
	Piano(double attackTime, double decayTime, double sustainLevel, double releaseTime);
	Piano(const std::map<double, double>& harmonics, double attackTime, double decayTime, double sustainLevel, double releaseTime);

	static const std::map<double, double> pianoHarmonics();

	double getDecayVolume(double time) const override;
	double getReleaseVolume(double time) const override;
};

class windInstrument : public Instrument
{
public:
	windInstrument(const std::map<double, double>& harmonics, double attackTime, double decayTime, double sustainLevel, double releaseTime);

	static const std::map<double, double> fluteHarmonics();
	static const std::map<double, double> clarinetHarmonics();
	static const std::map<double, double> trumpetHarmonics();

	double getAttackVolume(double time) const override;
	double getDecayVolume(double time) const override;
};

class Bell: public Instrument
{
public:
	Bell(double decayTime, double modulationIndex, double modulationFrequency);

	double getRealDuration(double duration) const override;
	double getDecayVolume(double time) const override;
};
