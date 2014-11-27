#include <cassert>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <vector>
#include "DebugOutput.h"
#include "Note.h"
#include "Track.h"

void printNote(std::ofstream* outputFile, const Note& note)
{
	(*outputFile)<<note.getFrequency()<<" "<<note.getDuration()<<" "<<note.getVolume();
}

void printMode(std::ofstream* outputFile, const Mode& mode)
{
	if (mode == MAJOR)
		(*outputFile)<<"MAJOR";
	if (mode == MINOR)
		(*outputFile)<<"MINOR";
}

void printChord(std::ofstream* outputFile, const Chord& chord)
{
	printNote(outputFile, chord.getNote());
	(*outputFile)<<" ";
	printMode(outputFile, chord.getMode());
}

void printNoteSequence(std::ofstream* outputFile, const std::vector<std::pair<Note, double> >& NoteSequence)
{
	int SequenceLength = NoteSequence.size();
	for (int i = 0; i < SequenceLength; ++i) {
		printNote(outputFile, NoteSequence[i].first);
		(*outputFile)<<" "<<NoteSequence[i].second<<"\n";
	}
}

void printChordSequence(std::ofstream* outputFile, const std::vector<std::pair<Chord, double> >& ChordSequence)
{
	int sequenceLength = ChordSequence.size();
	for (int i = 0; i < sequenceLength; ++i) {
		printChord(outputFile, ChordSequence[i].first);
		(*outputFile)<<" "<<ChordSequence[i].second<<"\n";
	}
}

void printInstrument(std::ofstream* outputFile, const Instrument& instrument)
{
	std::map<double, double> harmonics = instrument.getHarmonics();
	(*outputFile)<<harmonics.size()<<"\n";
	for (auto harmonic : harmonics)
		(*outputFile)<<harmonic.first<<" "<<harmonic.second<<"\n";
	(*outputFile)<<instrument.getModulationIndex0()<<" "<<instrument.getModulationRatio()<<"\n";
	(*outputFile)<<instrument.getAttackTime()<<" "<<
			instrument.getDecayTime()<<" "<<
			instrument.getSustainLevel()<<" "<<
			instrument.getReleaseTime()<<"\n";
}

void printTrack(std::ofstream* outputFile, const Track& track)
{
	int trackLength = track.getLength();
	for (int i = 0; i < trackLength; ++i) {
		assert(fabs(track.getValue(i)) <= MAX_AMPLITUDE);
		(*outputFile)<<track.getValue(i)<<"\n";
	}
}

void scanNoteSequence(FILE* inputFile, std::vector<std::pair<Note, double> >& NoteSequence)
{
	int NoteFrequency;
	double NoteDuration, Amplitude, Position;
	while (fscanf(inputFile, "%d%lf%lf%lf", &NoteFrequency, &NoteDuration, &Amplitude, &Position) == 4)
	        NoteSequence.push_back(std::make_pair(Note(NoteFrequency, NoteDuration, Amplitude), Position));
}
