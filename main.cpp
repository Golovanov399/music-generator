#include <cstdio>
#include <string>
#include <vector>
#include "Generator.h"
#include "DebugOutput.h"
#include "Track.h"

int main()
{
	std::vector<std::pair<Note, double> > Melody = generateMelody();
	FILE* OutputFile1 = fopen("NoteSequence.txt", "w");
	printNoteSequence(OutputFile1, Melody);
	fclose(OutputFile1);

	Track MelodyTrack(Melody);
	FILE* OutputFile2 = fopen("Track.txt", "w");
	printTrack(OutputFile2, MelodyTrack);
	fclose(OutputFile2);

	MelodyTrack.drop();

	return 0;
}
