#ifndef __Generator__
#define __Generator__

#include <vector>
#include <cstdlib>
#include <string>
#include "Note.h"

using namespace std;

class Generator{
private:
	int getNoteFrequency(const string& noteName, int octave);
	Chord generateTonicChord();
	int getNoteFrequencyByIndex(const Chord& tonicChord, int index);
	vector<pair<Chord, double> > generateChords(const Chord& tonicChord);
	vector<pair<Note, double> > generateAccompanement(const vector<pair<Chord, double> >& chords);
	vector<pair<Note, double> > generateMaintheme(const vector<pair<Chord, double> >& chords);

public:
	Generator(){}
	~Generator(){}
	vector<pair<Note, double> > generateMelody();
};

#endif
