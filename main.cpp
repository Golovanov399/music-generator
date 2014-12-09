#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Generator.h"
#include "Instrument.h"
#include "Track.h"
#include "DebugIO.h"
#include "Test.h"
#include "Effects.h"

int main()
{
	Generator2 gen;
	std::vector<std::pair<Note, double> > Melody = gen.generateMelody();

	std::ofstream OutputFile1("NoteSequence.txt", std::ofstream::out);
	printNoteSequence(&OutputFile1, Melody);

	Track melodyTrack(Melody, *randomInstrument());
	Tremolo effect(0.0, 0.0, 0.2, 3);
	Echo effect2(0.0, 0.0, 0.6, 0.4);
	//effect.applyEffect(melodyTrack);
	//effect2.applyEffect(melodyTrack);
	melodyTrack.normalize();

	std::ofstream OutputFile2("Track.txt", std::ofstream::out);
	printTrack(&OutputFile2, melodyTrack);

	melodyTrack.drop("sample.wav");

	//testInstrument(); 
	//testPiano();
	//testWindInstrument();
	return 0;
}
