#ifndef __Effects__

#define __Effects__

#include "Track.h"

class Effect
{
protected :
	double start_;
	double duration_;
	double power_;
public :
	Effect();
	Effect(	const double &start,
		const double &duration,
		const double &power);
	~Effect();
	void setStart(const double &value);
	void setDuration(const double &value);
	void setPower(const double &value);
	void setAll(	const double &start_value,
			const double &duration_value,
			const double &power_value);
	double getStart() const;
	double getDuration() const;
	double getPower() const;
	virtual void applyEffect(Track &sample) const = 0;	
};

class Echo : public Effect
{
public :
	Echo(	const double &start,
		const double &duration,
		const double &power);
	void applyEffect(Track &sample) const;
};

class Tremolo : public Effect
{
	double frequency_;
public :
	Tremolo(const double &start,
		const double &duration,
		const double &power,
		const double &frequency);
	void applyEffect(Track &sample) const;
};

#endif