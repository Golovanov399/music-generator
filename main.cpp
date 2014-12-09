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
	Melody melody = gen.generateMelody();

	std::ofstream OutputFile1("Accompaniment.txt", std::ofstream::out);
	printNoteSequence(&OutputFile1, melody.getAccompaniment());
	std::ofstream OutputFile2("MainTheme.txt", std::ofstream::out);
	printNoteSequence(&OutputFile2, melody.getMainTheme());

	//Track melodyTrack(melody, *randomInstrument(), *randomInstrument());
	Track melodyTrack(melody, Piano(), Piano());
	Tremolo effect(0.0, 0.0, 0.2, 3);
	Echo effect2(0.0, 0.0, 0.6, 0.4);
	//effect.applyEffect(melodyTrack);
	//effect2.applyEffect(melodyTrack);
	melodyTrack.normalize();

	std::ofstream OutputFile3("Track.txt", std::ofstream::out);
	printTrack(&OutputFile3, melodyTrack);

	melodyTrack.drop("sample.wav");

	//testInstrument(); 
	//testPiano();
	//testWindInstrument();
	return 0;
}
