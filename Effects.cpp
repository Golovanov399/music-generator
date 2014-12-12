#include <vector>
#include <cmath>
#include <iostream>

#include "Utility.h"
#include "Effects.h"
#include "CValues.h"

#define min(a, b) ((a < b ? a : b))

        
Effect::Effect() : 	start_(0),
			duration_(0),
			power_(0)
{
}

Effect::Effect(	const double &start,
		const double &duration,
		const double &power) :
		start_(start),
		duration_(duration),
		power_(power)
{
}

Effect::~Effect()
{
}

void Effect::setStart(const double &value)
{
	start_ = value;
}

void Effect::setDuration(const double &value)
{
	duration_ = value;
}

void Effect::setPower(const double &value)
{
	power_ = value;
}

void Effect::setAll(	const double &start_value,
			const double &duration_value,
			const double &power_value)
{
	start_ = start_value;
	duration_ = duration_value;
	power_ = power_value;
}

double Effect::getStart() const
{
	return start_;
}

double Effect::getDuration() const
{
	return duration_;
}

double Effect::getPower() const
{
	return power_;
}

Echo::Echo(	const double &start,
		const double &duration,
		const double &power,
		const double &delay) :
		Effect(start, duration, power),
		delay_(delay)
{
}

void Echo::applyEffect(Track &sample) const
{
	size_t 	buffer_size = (size_t)(delay_ * SAMPLE_RATE),
		buffer_ptr = 0,
		start = (size_t)(start_ * SAMPLE_RATE),
		finish = (duration_ <= 1e-6 ? sample.getLength() : 
			min((size_t)((start_ + duration_) * SAMPLE_RATE),
			sample.getLength()));
	std::vector<double> buffer(buffer_size);
	if (finish + buffer_size > sample.getLength())
		sample.resize(finish + buffer_size);
	for (size_t i = start; i < finish + buffer_size; i++)
	{
		double tmp = sample.getValue(i);
		sample.modifyValue(i, power_ * buffer[buffer_ptr]);
		buffer[buffer_ptr] = tmp;
		buffer_ptr = (buffer_ptr + 1) % buffer_size;
	}
}

Tremolo::Tremolo(	const double &start,
			const double &duration,
			const double &power,
			const double &frequency) :
			Effect(start, duration, power),
			frequency_(frequency)
{
}

void Tremolo::applyEffect(Track &sample) const
{
	size_t 	duration = (	duration_ <= 1e-6 ?
				sample.getLength() :
				(size_t)(duration_ * SAMPLE_RATE)),
		start = (size_t)(start_ * SAMPLE_RATE),
		sample_size = (	sample.getLength() < start + duration ?
				sample.getLength() :
				start + duration );
	SineOscillator tremoloOsc(SAMPLE_RATE, frequency_, 0);
	for (size_t i = start; i < sample_size; i++)
	{
//		std::cerr << tremoloOsc.getPhase() << ' ' << tremoloOsc.getModifier() << std::endl;
		double modifier = power_ * tremoloOsc.getModifier() - power_;
		sample.modifyValue(i, sample.getValue(i) * modifier);
		tremoloOsc.incrementPhase();
	}
}

Flanger::Flanger(	const double &start,
			const double &duration,
			const double &power,
			const double &frequency,
			const double &delay) :
			Effect(start, duration, power),
			frequency_(frequency),
			delay_(delay)
{
}

void Flanger::applyEffect(Track &sample) const
{
	size_t 	buffer_size = (size_t)(delay_ * SAMPLE_RATE);
	int	buffer_ptr = 0,
		delay = (int)(delay_ * SAMPLE_RATE);
	size_t	duration = (	duration_ <= 1e-6 ?
				sample.getLength() :
				(size_t)(duration_ * SAMPLE_RATE)),
		start = (size_t)(start_ * SAMPLE_RATE),
		finish = start + duration;
	double	depth = delay_ / 2.0;
	std::vector<double> buffer(buffer_size);
	if (finish + buffer_size > sample.getLength())
		sample.resize(finish + buffer_size);
	SineOscillator flangerOsc(SAMPLE_RATE, frequency_, 0);
	for (size_t i = start; i < finish + buffer_size; i++)
	{
		double flang_pos = depth + depth * flangerOsc.getModifier();
		int sample_pos = (int)(flang_pos * SAMPLE_RATE);
		if (sample_pos != 0)
		{
			sample.modifyValue(i, buffer[(buffer_ptr + buffer_size - sample_pos) % buffer_size] * power_);
			buffer[buffer_ptr] = sample.getValue(i);
		}
		else
		{
			buffer[buffer_ptr] = sample.getValue(i);
			sample.modifyValue(i, sample.getValue(i) * power_);
		}
		buffer_ptr = (buffer_ptr + 1) % buffer_size;
		flangerOsc.incrementPhase();
	}
}

Distortion::Distortion(	const double &start,
			const double &duration,
			const double &power,
			const double &level) :
			Effect(start, duration, power),
			level_(level)
{
}

void Distortion::applyEffect(Track &sample) const
{
	sample.normalize();
	size_t 	duration = (	duration_ <= 1e-6 ?
				sample.getLength() :
				(size_t)(duration_ * SAMPLE_RATE)),
		start = (size_t)(start_ * SAMPLE_RATE),
		sample_size = (	sample.getLength() < start + duration ?
				sample.getLength() :
				start + duration );
	for (size_t i = start; i < sample_size; i++)
	{
		double val = fabs(sample.getValue(i)) / MAX_AMPLITUDE;
		if (val <= 1e-6 + level_) continue;
		double mod = ((val - level_) * power_ + level_ - val) / val;
		//std::cerr << mod << std::endl;
		sample.modifyValue(i, sample.getValue(i) * mod);
	}
}