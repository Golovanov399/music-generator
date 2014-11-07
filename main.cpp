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
	Generator1* gen = new Generator1();
	std::vector<std::pair<Note, double> > Melody = gen->generateMelody();
	delete gen;
/*	FILE* OutputFile1 = fopen("NoteSequence.txt", "w");
	printNoteSequence(OutputFile1, Melody);
	fclose(OutputFile1);

	Instrument noInstrument(std::vector<double>(0), 0.0, 0.0, 1.0, 0.0);
	vector<double> harmonics(10);
	for (int i = 0; i < 10; i++)
		harmonics[i] = (i & 2 ? (1.0 / (double)(i + 2)) : 0);
	Instrument sawInstrument(harmonics, 0.0, 0.0, 1.0, 0.0);

	// Track MelodyTrack(Melody, noInstrument);
	Track MelodyTrack(Melody, sawInstrument);
	MelodyTrack.normalize();

	FILE* OutputFile2 = fopen("Track.txt", "w");
	printTrack(OutputFile2, MelodyTrack);
	fclose(OutputFile2);
*/

	Track MelodyTrack(Melody, testInstrument());	
	MelodyTrack.drop();

	return 0;
}
