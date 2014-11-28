#pragma once

#include <fstream>
#include <vector>
#include "Note.h"
#include "Instrument.h"

class Track
{
	std::vector<double> wave_;

public:
	Track();
	Track(const std::vector<double> &wave);
	Track(const Note& element,
		const Instrument& instrument,
		double phase);
	Track(const std::vector<std::pair<Note, double>> &sequence, 
		const Instrument &instrument); // naive constructor;

	int getLength() const;
	double getValue(const size_t &index) const;
	void modifyValue(const size_t &index, const double &value);

	Track add(int offset,
			const Track& delta) const;
	void addToSelf(int offset,
			const Track& delta);
	void normalize();
	void drop(const char* outputFileName) const;
	void resize(const size_t &size);
};
