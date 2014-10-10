#ifndef __DebugOutput__

#define __DebugOutput__

#include <cstdio>
#include <vector>
#include "Note.h"
#include "Track.h"

void printNote(FILE* OutputFile, const Note& note);
void printMode(FILE* OutputFile, const Mode& mode);
void printChord(FILE* OutputFile, const Chord& chord);
void printNoteSequence(FILE* OutputFile, const std::vector<std::pair<Note, double> >& NoteSequence);
void printChordSequence(FILE* OutputFile, const std::vector<std::pair<Chord, double> >& ChordSequence);
void printTrack(FILE* OutputFile, const Track& track);
void scanNoteSequence(FILE* InputFile, std::vector<std::pair<Note, double> >& NoteSequence);

#endif
