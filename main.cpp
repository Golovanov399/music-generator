#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Generator.h"
#include "Instrument.h"
#include "Track.h"
#include "DebugIO.h"
#include "Test.h"

int main()
{
	Generator1 gen;
	std::vector<std::pair<Note, double> > Melody = gen.generateMelody();

	std::ofstream OutputFile1("NoteSequence.txt", std::ofstream::out);
	printNoteSequence(&OutputFile1, Melody);

	Track melodyTrack(Melody, Piano(0.01, 0.5));
	melodyTrack.normalize();

	std::ofstream OutputFile2("Track.txt", std::ofstream::out);
	printTrack(&OutputFile2, melodyTrack);

	melodyTrack.drop("sample.wav");

	//testInstrument();
	//testPiano();

/*	for (int i = 1; i <= 21; ++i)
	{	
		std::ifstream infile("Piano" + std::to_string(i) + ".txt", std::ifstream::in);
		Piano piano = scanPiano(&infile);
		std::vector<std::pair<Note, double> > Melody(1, std::pair<Note, double>(Note(9, 0.003, 25), 0));
		Track melodyTrack(Melody, piano);
		melodyTrack.normalize();
		std::ofstream OutputFile2("Piano" + std::to_string(i) + " track.txt", std::ofstream::out);
		printTrack(&OutputFile2, melodyTrack);
	}
*/
	return 0;
}
