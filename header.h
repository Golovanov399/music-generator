const double REAL_FREQUENCY = 1.0;

enum Mode {major, minor};

class Note {
	int frequency_;			
	double duration_;		
	double volume_;
	
public :
	Note();
	Note(int frequency, double duration, double volume) {frequency_ = frequency;	duration_ = duration; volume_ = volume;}
	double getRealFrequency() {return frequency_*REAL_FREQUENCY;}
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
} ;
