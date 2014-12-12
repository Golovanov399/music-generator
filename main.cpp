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

	Track melodyTrack(melody, *randomInstrument(), *randomInstrument());
	//Track melodyTrack(melody, windInstrument::Organ(2), windInstrument::Organ(2));
	Tremolo effect(0.0, 0.0, 0.3, 3);
	Echo effect2(0.0, 0.0, 0.6, 0.4);
	Flanger effect3(0.0, 0.0, 0.3, 1, 0.01);
	Distortion effect4(0.0, 0.0, 0.0, 0.2);
	//effect.applyEffect(melodyTrack);
	//effect2.applyEffect(melodyTrack);
	//effect3.applyEffect(melodyTrack);
	//effect4.applyEffect(melodyTrack);
	int effect_type = rand() % 5;
	if (effect_type == 0) effect.applyEffect(melodyTrack);
	if (effect_type == 1) effect2.applyEffect(melodyTrack);
	if (effect_type == 2) effect3.applyEffect(melodyTrack);
	if (effect_type == 3) effect4.applyEffect(melodyTrack);
	melodyTrack.normalize();

	std::ofstream OutputFile3("Track.txt", std::ofstream::out);
	printTrack(&OutputFile3, melodyTrack);

	melodyTrack.drop("sample.wav");

	//testInstrument(); 
	//testPiano();
	//testWindInstrument();
	return 0;
}
