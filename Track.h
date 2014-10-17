#ifndef __Track__

#define __Track__

#include <vector>
#include "Note.h"

<<<<<<< HEAD
const double attack_value = 0.2;
const double decay_value = 0.2;
const double sustain_value = 0.2;
const double release_value = 0.2;
=======
const int attack_value = 0.2;
const int decay_value = 0;
const int sustain_value = 1;
const int release_value = 0.2;
>>>>>>> origin/master

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

double ADSR(int index, double arg);

#endif
