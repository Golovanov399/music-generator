#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "Note.h"
#include "Track.h"

void printNote(std::ofstream* outputFile, const Note& note);
void printMode(std::ofstream* outputFile, const Mode& mode);
void printChord(std::ofstream* outputFile, const Chord& chord);
void printNoteSequence(std::ofstream* outputFile, const std::vector<std::pair<Note, double> >& NoteSequence);
void printChordSequence(std::ofstream* outputFile, const std::vector<std::pair<Chord, double> >& ChordSequence);
void printInstrument(std::ofstream* outputFile, const Instrument& instrument);
void printTrack(std::ofstream* outputFile, const Track& track);

std::vector<std::pair<Note, double> > scanNoteSequence(std::istream* inputFile);
Instrument scanInstrument(std::istream* inputFile);
Piano scanPiano(std::istream* inputFile);
