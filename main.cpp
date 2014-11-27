#include <cstdio>
#include <fstream>
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

	std::ofstream OutputFile1("NoteSequence.txt", std::ofstream::out);
	printNoteSequence(&OutputFile1, Melody);

	Track MelodyTrack(Melody, windInstrument(windInstrument::fluteHarmonics(), 0.125, 0, 1.0, 0.01));
	MelodyTrack.normalize();

	std::ofstream OutputFile2("Track.txt", std::ofstream::out);
	printTrack(&OutputFile2, MelodyTrack);

	MelodyTrack.drop("sample.wav");

	//testPiano();

	return 0;
}
