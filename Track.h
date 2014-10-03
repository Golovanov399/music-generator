#ifndef __Track__

#define __Track__

#include <vector>

class Track
{
	std::vector<double> wave_;

public:
	Track();
	Track(const std::vector<double>& wave);
//	Track(const std::vector<std::pair<Note, double> >& sequence); - ToDo
	int getLength() const;
	double getValue(int index) const;

	Track add(int offset, const Track& delta) const;
	void drop() const;
};

#endif
