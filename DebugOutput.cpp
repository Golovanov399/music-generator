#include <cstdio>
#include <vector>
#include "DebugOutput.h"
#include "Note.h"
#include "Track.h"

void printNote(FILE* OutputFile, const Note& note)
{
	fprintf(OutputFile, "%d %f %f", note.getFrequency(), note.getDuration(), note.getVolume());
	return;
}

void printMode(FILE* OutputFile, const Mode& mode)
{
	if (mode == MAJOR)
		fprintf(OutputFile, "MAJOR");
	if (mode == MINOR)
		fprintf(OutputFile, "MINOR");
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

void scanNoteSequence(FILE* InputFile, std::vector<std::pair<Note, double> >& NoteSequence)
{
	int NoteFrequency; double NoteDuration, Amplitude, Position;
	while (fscanf(InputFile, "%d%lf%lf%lf", &NoteFrequency, &NoteDuration, &Amplitude, &Position) == 4)
	        NoteSequence.push_back(std::make_pair(Note(NoteFrequency, NoteDuration, Amplitude), Position));
}
