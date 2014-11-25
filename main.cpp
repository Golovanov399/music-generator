#include <cstdio>
#include <string>
#include <vector>
#include "Generator.h"
#include "Instrument.h"
#include "Track.h"
#include "DebugOutput.h"
#include "Test.h"

int main()
{
	Generator1 gen;
	std::vector<std::pair<Note, double> > Melody = gen.generateMelody();
/*
	std::vector<std::pair<Note, double> > Melody;
	Melody.push_back(std::pair<Note, double>(Note(9, 1.0, 25), 0));
	//Melody.push_back(std::pair<Note, double>(Note(10, 1.0, 25), 1));
*/
	FILE* OutputFile1 = fopen("NoteSequence.txt", "w");
	printNoteSequence(OutputFile1, Melody);
	fclose(OutputFile1);

	Track MelodyTrack(Melody, Instrument());
	MelodyTrack.normalize();

	FILE* OutputFile2 = fopen("Track.txt", "w");
	printTrack(OutputFile2, MelodyTrack);
	fclose(OutputFile2);

	MelodyTrack.drop();

	return 0;
}
