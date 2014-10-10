#ifndef __Generator__
#define __Generator__

#include <vector>
#include <cstdlib>
#include <string>
#include <ctime>
#include "Note.h"

using namespace std;

int getNoteFrequency(const string& noteName, int octave = 0);

Chord generateTonicChord();

int getNoteFrequencyByIndex(const Chord& tonicChord, int index);

vector<pair<Chord, double> > generateChords(const Chord& tonicChord);

vector<pair<Note, double> > generateAccompanement(const vector<pair<Chord, double> >& chords);

vector<pair<Note, double> > generateMaintheme(const vector<pair<Chord, double> >& chords);

vector<pair<Note, double> > generateMelody();

#endif
