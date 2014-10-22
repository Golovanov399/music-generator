#ifndef __Track__

#define __Track__

#include <vector>
#include "Note.h"
#include "Instrument.h"

const double SECONDS_IN_BAR = 1.0;

class Track
{
	std::vector<double> wave_;

public:
	Track();
	Track(const std::vector<double>& wave);
	Track(const Note& element, const Instrument& instrument);
	Track(const std::vector<std::pair<Note, double> >& sequence, const Instrument& instrument); // naive constructor;
	int getLength() const;
	double getValue(int index) const;

	Track add(int offset, const Track& delta) const;
	void addToSelf(int offset, const Track& delta);
	void normalize();
	void drop() const;
};

#endif
