#include <cmath>
#include "Utility.h"
#include "CValues.h"

void Oscillator::updatePhaseIncrement()
{
	phase_increment_ = w_ * frequency_;
}

Oscillator::Oscillator() :
			w_(TWO_PI / SAMPLE_RATE),
			frequency_(1.0),
			phase_(0.0)
{
	updatePhaseIncrement();
}

Oscillator::Oscillator(const double &sample_rate,
			const double &frequency,
			const double &phase) :
			w_(TWO_PI * sample_rate),
			frequency_(frequency),
			phase_(phase)
{
	updatePhaseIncrement();
}

Oscillator::~Oscillator() {}

void Oscillator::setSampleRate(const double &value)
{
	w_ = TWO_PI / value;
	updatePhaseIncrement();
}

void Oscillator::setFrequency(const double &value)
{
	frequency_ = value;
	updatePhaseIncrement();
}

void Oscillator::setPhase(const double &value)
{
	phase_ = value;
}

void Oscillator::setAll(const double &sample_rate_value,
		const double &frequency_value,
		const double &phase_value)
{
	w_ = TWO_PI / sample_rate_value;
	frequency_ = frequency_value;
	phase_ = phase_value;
	updatePhaseIncrement();
}

double Oscillator::getSampleRate() const
{
	return TWO_PI / w_;
}

double Oscillator::getFrequency() const
{
	return frequency_;
}

double Oscillator::getPhase() const
{
	return phase_;
}

void Oscillator::incrementPhase()
{
	phase_ += phase_increment_;
}

double SineOscillator::getModifier() const
{
	return sin(phase_);	
}