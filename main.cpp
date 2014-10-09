#include <cstdio>
#include <vector>
#include "Generator.h"
#include "DebugOutput.h"
#include "Track.h"

int main()
{
	std::vector<std::pair<Note, double> > Melody = generateMelody();
	FILE* OutputFile1 = fopen("NoteSequence.txt", "w");
	printNoteSequence(OutputFile1, Melody);

	Track MelodyTrack(Melody);
	FILE* OutputFile2 = fopen("Track.txt", "w");
	printTrack(OutputFile2, MelodyTrack);

	MelodyTrack.drop();

	return 0;
}
