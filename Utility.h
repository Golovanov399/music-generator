#ifndef __Utility__

#define __Utility__

class Oscillator
{
protected :
        double w_; // 2*PI / SAMPLE_RATE;
	double frequency_;
	double phase_;
	double phase_increment_;
	void updatePhaseIncrement();
public :
	Oscillator();
	Oscillator(	const double &sample_rate,
			const double &frequency,
			const double &phase);
	~Oscillator();
	void setSampleRate(const double &value);
	void setFrequency(const double &value);
	void setPhase(const double &value);
	void setAll(	const double &sample_rate_value,
			const double &frequency_value,
			const double &phase_value);
	double getSampleRate() const;
	double getFrequency() const;
	double getPhase() const;
	void incrementPhase();
	virtual double getModifier() const = 0;
};

class SineOscillator : public Oscillator
{
public :
	SineOscillator();
	SineOscillator(	const double &sample_rate,
			const double &frequency,
			const double &phase);
	double getModifier() const;
};

#endif