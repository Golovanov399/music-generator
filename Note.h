#ifndef __Note__

#define __Note__

const double REAL_FREQUENCY = 440.0;
const double MAX_AMPLITUDE = 100.0;
const double MIN_AMPLITUDE = 0.0;

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
} ;

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
} ;

#endif
