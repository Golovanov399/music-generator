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

void printNoteSequence(const std::vector<std::pair<Note, double> >& NoteSequence, const char* FileName)
{
	int SequenceLength = NoteSequence.size();
	FILE* OutputFile = fopen(FileName, "w");
	for (int i = 0; i < SequenceLength; ++i) {
		Note CurrentNote = NoteSequence[i].first;
		fprintf(OutputFile, "%d %f %f %f\n", CurrentNote.getFrequency(), CurrentNote.getDuration(), CurrentNote.getVolume(), NoteSequence[i].second);
	}
	return;
}

void printChordSequence(const std::vector<std::pair<Chord, double> >& ChordSequence, const char* FileName)
{
	int SequenceLength = ChordSequence.size();
	FILE* OutputFile = fopen(FileName, "w");
	for (int i = 0; i < SequenceLength; ++i) {
		Chord CurrentChord = ChordSequence[i].first;
		fprintf(OutputFile, "%d %f %f %s %f\n", CurrentChord.getFrequency(), CurrentChord.getDuration(), CurrentChord.getVolume(), getModeName(CurrentChord.getMode()), ChordSequence[i].second);
	}
	return;
}

void printTrack(const Track& track, const char* FileName)
{
	int TrackLength = track.getLength();
	FILE* OutputFile = fopen(FileName, "w");
	for (int i = 0; i < TrackLength; ++i) {
		fprintf(OutputFile, "%f\n", track.getValue(i));
	}
}

#endif
