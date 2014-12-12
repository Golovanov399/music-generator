#ifndef __Generator__
#define __Generator__

#include <vector>
#include <cstdlib>
#include <string>
#include "Note.h"

using namespace std;

namespace generator{
	int getNoteFrequency(const string& noteName, int octave);
	Chord generateTonicChord();
	int getNoteFrequencyByIndex(const Chord& tonicChord, int index);
	int getNoteIndex(const Chord& tonicChord, const Note& note);
	int getNextNoteFrequency(const Chord& tonicChord, const Note& note, int additor);
}

class Generator{
public:
	double basicChordLength;

	Generator();
	~Generator(){}
	Melody generateMelody() const;

  // FixMe: use typedefs
	virtual vector<pair<Chord, double> > generateChords(const Chord& tonicChord) const = 0;
	virtual vector<pair<Note, double> > generateAccompaniment(const vector<pair<Chord, double> >& chords, const Chord& tonicChord) const = 0;
	virtual vector<pair<Note, double> > generateMaintheme(const vector<pair<Chord, double> >& chords, const Chord& tonicChord) const = 0;
};

class Generator1 : public Generator{
public:
	Generator1(){}
	~Generator1(){}
	vector<pair<Chord, double> > generateChords(const Chord& tonicChord) const;
	vector<pair<Note, double> > generateAccompaniment(const vector<pair<Chord, double> >& chords, const Chord& tonicChord) const;
	vector<pair<Note, double> > generateMaintheme(const vector<pair<Chord, double> >& chords, const Chord& tonicChord) const;
};

class Generator2 : public Generator{
public:
	Generator2(){}
	~Generator2(){}
	vector<pair<Chord, double> > generateChords(const Chord& tonicChord) const;
	vector<pair<Note, double> > generateAccompaniment(const vector<pair<Chord, double> >& chords, const Chord& tonicChord) const;
	vector<pair<Note, double> > generateMaintheme(const vector<pair<Chord, double> >& chords, const Chord& tonicChord) const;
};

#endif
