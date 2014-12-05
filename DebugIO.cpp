#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include "DebugIO.h"
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

std::vector<std::pair<Note, double> > scanNoteSequence(std::istream* inputFile)
{
	int numberOfNotes;
	(*inputFile)>>numberOfNotes;

	std::vector<std::pair<Note, double> > noteSequence;
	for (int i = 0; i < numberOfNotes; ++i)
	{
		int Frequency;
		double Duration, Amplitude, Position;
		(*inputFile)>>Frequency>>Duration>>Amplitude>>Position;
	        noteSequence.push_back(std::pair<Note, double>(Note(Frequency, Duration, Amplitude), Position));
	}

	return noteSequence;
}

Instrument scanInstrument(std::istream* inputFile)
{
	int numberOfHarmonics;
	(*inputFile)>>numberOfHarmonics;

	std::map<double, double> harmonics;
	for (int i = 0; i < numberOfHarmonics; ++i)
	{
		double harmonic, harmonicIndex;
		(*inputFile)>>harmonic>>harmonicIndex;
		harmonics[harmonic] = harmonicIndex;
	}	

	double A, D, S, R;
	(*inputFile)>>A>>D>>S>>R;

	return Instrument(harmonics, A, D, S, R);
}

Piano scanPiano(std::istream* inputFile)
{
	int numberOfHarmonics;
	(*inputFile)>>numberOfHarmonics;

	std::map<double, double> harmonics;
	for (int i = 0; i < numberOfHarmonics; ++i)
	{
		double harmonic, harmonicIndex;
		(*inputFile)>>harmonic>>harmonicIndex;
		harmonics[harmonic] = harmonicIndex;
	}	

	return Piano(harmonics, 0.006);
}
