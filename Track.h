#ifndef __Track__

#define __Track__

#include <vector>
#include "Note.h"

const int SAMPLE_RATE = 44100;
const double SECONDS_IN_BAR = 1.0;
const double attack_value = 0.2;
const double decay_value = 0.2;
const double sustain_value = 0.2;
const double release_value = 0.2;

class Track
{
	std::vector<double> wave_;

public:
	Track();
	Track(const std::vector<double>& wave);
	Track(const Note& element);
	Track(const std::vector<std::pair<Note, double> >& sequence); // naive constructor;
	int getLength() const;
	double getValue(int index) const;

	Track add(int offset, const Track& delta) const;
	void addToSelf(int offset, const Track& delta);
	void normalize();
	void drop() const;
};

double ADSR(int index, double time, double duration);

#endif
