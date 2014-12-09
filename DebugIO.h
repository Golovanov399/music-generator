#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "Note.h"
#include "Track.h"

void printNote(std::ostream* outputFile, const Note& note);
void printMode(std::ostream* outputFile, const Mode& mode);
void printChord(std::ostream* outputFile, const Chord& chord);
void printNoteSequence(std::ostream* outputFile, const std::vector<std::pair<Note, double> >& NoteSequence);
void printChordSequence(std::ostream* outputFile, const std::vector<std::pair<Chord, double> >& ChordSequence);
void printInstrument(std::ostream* outputFile, const Instrument& instrument);
void printTrack(std::ostream* outputFile, const Track& track);

std::vector<std::pair<Note, double> > scanNoteSequence(std::istream* inputFile);
Instrument scanInstrument(std::istream* inputFile);
Piano scanPiano(std::istream* inputFile);
windInstrument scanWindInstrument(std::istream* inputFile);
