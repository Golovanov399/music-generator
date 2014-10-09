#ifndef __DebugOutput__

#define __DebugOutput__

#include <cstdio>
#include <vector>
#include "Note.h"
#include "Track.h"

const char* getModeName(Mode m)
{
	if (m == major)
		return "major";
	if (m == minor)
		return "minor";
	return "";
}

void printNote(FILE* OutputFile, const Note& note)
{
	fprintf(OutputFile, "%d %f %f", note.getFrequency(), note.getDuration(), note.getVolume());
	return;
}

void printMode(FILE* OutputFile, const Mode& mode)
{
	if (mode == major)
		fprintf(OutputFile, "major");
	if (mode == minor)
		fprintf(OutputFile, "minor");
	return;
}

void printChord(FILE* OutputFile, const Chord& chord)
{
	printNote(OutputFile, chord.getNote());
	fprintf(OutputFile, " ");
	printMode(OutputFile, chord.getMode());
	return;
}

void printNoteSequence(FILE* OutputFile, const std::vector<std::pair<Note, double> >& NoteSequence)
{
	int SequenceLength = NoteSequence.size();
	for (int i = 0; i < SequenceLength; ++i) {
		printNote(OutputFile, NoteSequence[i].first);
		fprintf(OutputFile, " %f\n", NoteSequence[i].second);
	}
	return;
}

void printChordSequence(FILE* OutputFile, const std::vector<std::pair<Chord, double> >& ChordSequence)
{
	int SequenceLength = ChordSequence.size();
	for (int i = 0; i < SequenceLength; ++i) {
		printChord(OutputFile, ChordSequence[i].first);
		fprintf(OutputFile, " %f\n", ChordSequence[i].second);
	}
	return;
}

void printTrack(FILE* OutputFile, const Track& track)
{
	int TrackLength = track.getLength();
	for (int i = 0; i < TrackLength; ++i) {
		fprintf(OutputFile, "%f\n", track.getValue(i));
	}
}

#endif
