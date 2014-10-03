#ifndef __Note__

#define __Note__

const double REAL_FREQUENCY = 440.0;
const double MAX_AMPLITUDE = 100.0;
const double MIN_AMPLITUDE = 0.0;

enum Mode {major, minor};

class Note {
	int frequency_;			
	double duration_;		
	double volume_;
	
public :
	Note();
	Note(int frequency, double duration, double volume) {frequency_ = frequency;	duration_ = duration; volume_ = volume;}
	double getRealFrequency() {return pow(2, 1.0 * (frequency_ - 9) / 12)*REAL_FREQUENCY;}
	double getDuration() {return duration_ ;}
	double getVolume() {return volume_;}
} ;

class Chord {
	Note note_;
	Mode mode_;
	
public :
	Chord(Note note, Mode mode) {
		note_ = note;
		mode_ = mode;
	}
	Mode getMode() {return mode_;}
	Note getNote() {return note_;}
} ;

#endif
