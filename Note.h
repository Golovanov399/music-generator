#pragma once

#ifndef __Note__

#define __Note__

#include <cmath>	//for pow(x, y)

const double REAL_FREQUENCY = 440.0;
const double MAX_AMPLITUDE = 100.0;
const double MIN_AMPLITUDE = 0.0;

enum Mode {major, minor};

class Note {
	int frequency_;			
	double duration_;		
	double volume_;
	
public :
	Note(){frequency_ = 440; duration_ = 1; volume_ = MAX_AMPLITUDE;};
	Note(int frequency, double duration = 1, double volume = MAX_AMPLITUDE) {frequency_ = frequency;	duration_ = duration; volume_ = volume;}
	double getRealFrequency() const {return pow(2, 1.0 * (frequency_ - 9) / 12)*REAL_FREQUENCY;}
	double getFrequency() const {return frequency_ ;}
	double getDuration() const {return duration_ ;}
	double getVolume() const {return volume_;}
} ;

class Chord {
	Note note_;
	Mode mode_;
	
public :
	Chord(Note note, Mode mode) {
		note_ = note;
		mode_ = mode;
	}
	Mode getMode() const {return mode_;}
	Note getNote() const {return note_;}
} ;

#endif
