#ifndef __Track__

#define __Track__

#include <vector>
#include "Note.h"

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

#endif
