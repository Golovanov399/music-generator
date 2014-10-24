#include <cstdio>
#include <string>
#include <vector>
#include "Generator.h"
#include "Instrument.h"
#include "Track.h"
#include "DebugOutput.h"

int main()
{
	Generator1* gen = new Generator1();
	std::vector<std::pair<Note, double> > Melody = gen->generateMelody();
	delete gen;
	FILE* OutputFile1 = fopen("NoteSequence.txt", "w");
	printNoteSequence(OutputFile1, Melody);
	fclose(OutputFile1);

	Instrument noInstrument(std::vector<double>(0), 0.0, 0.0, 1.0, 0.0);
	// Instrument unknownInstrument1(std::vector<double>(0), 0.2, 0.2, 0.3, 0.2);

	Track MelodyTrack(Melody, noInstrument);
	// Track MelodyTrack(Melody, unknownInstrument1);
	MelodyTrack.normalize();

	FILE* OutputFile2 = fopen("Track.txt", "w");
	printTrack(OutputFile2, MelodyTrack);
	fclose(OutputFile2);
	
	MelodyTrack.drop();

	return 0;
}
