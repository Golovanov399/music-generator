#ifndef __Track__

#define __Track__

#include <vector>
#include "Note.h"

const attack_value = 0.2;
const decay_value = 0;
const sustain_value = 1;
const release_value = 0.2;

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
	void drop() const;
};

double ADSR(double arg);

#endif
