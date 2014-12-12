#pragma once

#include <utility>
#include <vector>
#include "CValues.h"

enum Mode {MAJOR, MINOR};

class Note {
	int frequency_;
	double duration_;
	double volume_;

public :
	Note();
	Note(int frequency, double duration = 1, double volume = MAX_AMPLITUDE / 4);
	double getRealFrequency() const;
	int getFrequency() const;
	double getDuration() const;
	double getVolume() const;

	void setVolume(double volume);
};

class Chord {
	Note note_;
	Mode mode_;

public :
	Chord(Note note, Mode mode);
	Mode getMode() const;
	Note getNote() const;
	int getFrequency() const;
	double getDuration() const;
	double getVolume() const;
};

typedef std::vector<std::pair<Note, double> > noteSequence;

class Melody {
	noteSequence accompaniment_;
	noteSequence mainTheme_;
	double secondsInBar_;

public:
	Melody(const noteSequence& sequence1, const noteSequence& dequence2, double number);
	noteSequence getAccompaniment() const;
	noteSequence getMainTheme() const;
	double getSecondsInBar() const;
};
