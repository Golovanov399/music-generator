#pragma once

#include <cstdlib>
#include <map>
#include <memory>
#include <vector>

struct Harmonics
{
	std::map<double, double> harmonics;
	double normalizeIndex;

	Harmonics();
	Harmonics(const std::map<double, double>& harmonics, double normalizeindex);
};

class Instrument
{
protected:
	Harmonics harmonics_;	
	double attackTime_;
	double decayTime_;
	double sustainLevel_;
	double releaseTime_;
public:
	Instrument();
	Instrument(const Harmonics& harmonics);
	Instrument(double attackTime, double decayTime, double sustainLevel, double releaseTime);
	Instrument(const Harmonics& harmonics, double attackTime, double decayTime, double sustainLevel, double releaseTime);

	static const int maxHarmonicNumber();
	static const double minAttack();
	static const double minRelease();
	static const double minReleaseLevel();
	static const double minModulationIndex();

	std::map<double, double> getHarmonics() const;	
	double getAttackTime() const;
	double getDecayTime() const;
	double getSustainLevel() const;
	double getReleaseTime() const;

	static const int numberOfWaves();
	static const Harmonics sawHarmonics(int numberOfHarmonics);
	static const Harmonics squareHarmonics(int numberOfHarmonics);
	static const Harmonics triangleHarmonics(int numberOfHarmonics);
	static const Instrument randomWave();

	double getRealDuration(double duration) const;
	virtual double getAttackVolume(double time) const;
	virtual double getDecayVolume(double time) const;
	virtual double getReleaseVolume(double lastLevel, double time) const;
	
	double ADSR(double frequency, double time, double duration) const;	
	double getModuloIndexValue(double time, double duration) const;

	double getWaveValue(double frequency, double phase, double volume, double time, double duration) const;
};

class Piano: public Instrument
{
public:
	Piano();
	Piano(const Harmonics& harmonics);

	static const Harmonics pianoHarmonics();
	static const double pianoAttack();
	static const double pianoRelease();

	double getAttackVolume(double time) const override;
	double getReleaseVolume(double lastLevel, double time) const override;
};

class windInstrument : public Instrument
{
public:
	windInstrument(const Harmonics& harmonics, double attackTime, double decayTime, double sustainLevel, double releaseTime);

	static const int numberOfFlutes();
	static const windInstrument Flute(int mode);
	static const windInstrument randomFlute();
	
	static const int numberOfOrgans();
	static const windInstrument Organ(int mode);
	static const windInstrument randomOrgan();

	double getAttackVolume(double time) const override;
	double getReleaseVolume(double lastLevel, double time) const override;
};

std::unique_ptr<Instrument> randomInstrument();
