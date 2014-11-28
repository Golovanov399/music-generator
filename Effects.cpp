#include <vector>
#include "Utility.h"
#include "Effects.h"
#include "CValues.h"
        
Effect::Effect() : start_(0),
			duration_(0),
			power_(0)
{
}

Effect::Effect(const double &start,
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

void Effect::setAll(const double &start_value,
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

void Echo::applyEffect(Track &sample) const
{
	size_t buffer_size = (size_t)(duration_ * SAMPLE_RATE),
		buffer_ptr = 0,
		start = (size_t)(start_ * SAMPLE_RATE);
	std::vector<double> buffer(buffer_size);
	size_t sample_size = sample.getLength();
	sample.resize(sample_size + buffer_size);
	for (size_t i = start; i < sample.getLength(); i++)
	{
		double tmp = sample.getValue(i);
		sample.modifyValue(i, power_ * buffer[buffer_ptr]);
		buffer[buffer_ptr] = tmp;
	}
}
