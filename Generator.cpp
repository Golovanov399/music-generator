#include <vector>
#include <cstdlib>
#include <string>
#include <ctime>
#include "Generator.h
#include "Note.h"

using namespace std;

int getNoteFrequency(const string& noteName, int octave = 0){
	int currentFrequency = 0;
	switch (noteName[0]){
		case 'C':
			currentFrequency = 0;
		case 'D':
			currentFrequency = 2;
		case 'E':
			currentFrequency = 4;
		case 'F':
			currentFrequency = 5;
		case 'G':
			currentFrequency = 7;
		case 'A':
			currentFrequency = 9;
		case 'B':
			currentFrequency = 10;
		case 'H':
			currentFrequency = 11;
	}
	if (noteName.length() > 1){
		if (noteName[1] == '#'){
			currentFrequency++;
		} else {
			currentFrequency--;
		}
	}
	return currentFrequency + 12 * octave;
}

Chord generateTonicChord(){
	return Chord(Note(getNoteFrequency("A")), minor);
}

int getNoteFrequencyByIndex(const Chord& tonicChord, int index){
	// 1-indexation here
	if (tonicChord.getMode() == major){
		switch(index){
			case 1:
				return tonicChord.getNote().getFrequency() + 0;
			case 2:
				return tonicChord.getNote().getFrequency() + 2;
			case 3:
				return tonicChord.getNote().getFrequency() + 4;
			case 4:
				return tonicChord.getNote().getFrequency() + 5;
			case 5:
				return tonicChord.getNote().getFrequency() + 7;
			case 6:
				return tonicChord.getNote().getFrequency() + 9;
			case 7:
				return tonicChord.getNote().getFrequency() + 11;
			default:
				return tonicChord.getNote().getFrequency() + 12;
		}
	} else {
		switch(index){
			case 1:
				return tonicChord.getNote().getFrequency() + 0;
			case 2:
				return tonicChord.getNote().getFrequency() + 2;
			case 3:
				return tonicChord.getNote().getFrequency() + 3;
			case 4:
				return tonicChord.getNote().getFrequency() + 5;
			case 5:
				return tonicChord.getNote().getFrequency() + 7;
			case 6:
				return tonicChord.getNote().getFrequency() + 8;
			case 7:
				return tonicChord.getNote().getFrequency() + 10;
			default:
				return tonicChord.getNote().getFrequency() + 12;
		}		
	}
}

vector<pair<Chord, double> > generateChords(const Chord& tonicChord){
	/* 	Each element of vector<pair<Chord, double>>
		is a pair of a Chord and its appearance time
	*/
	vector<pair<Chord, double> > chords;
	int tonicFrequency = tonicChord.getNote().getFrequency();
	srand(time(NULL));
	int count = 1 << (rand() % 2 + 2);
	double chordLength = 1;
	vector<Chord> combo;
	combo.push_back(Chord(getNoteFrequencyByIndex(tonicChord, 1), minor));
	combo.push_back(Chord(getNoteFrequencyByIndex(tonicChord, 6), major));
	combo.push_back(Chord(getNoteFrequencyByIndex(tonicChord, 3), major));
	combo.push_back(Chord(getNoteFrequencyByIndex(tonicChord, 7), major));
	for (int i = 0; i < count; i++){
		for (int j = 0; j < combo.size(); j++){
			chords.push_back(make_pair(combo[j], (combo.size() * i + j) * chordLength));
		}
	}
	return chords;
}

vector<pair<Note, double> > generateAccompanement(const vector<pair<Chord, double> >& chords){
	vector<pair<Note, double> > accompanement;
	for (int i = 0; i < chords.size(); i++){
		for (int j = 1; j <= 5; j += 2){
			accompanement.push_back(make_pair(getNoteFrequencyByIndex(chords[i].first, j), chords[i].second));
		}
	}
	return accompanement;
}

vector<pair<Note, double> > generateMaintheme(const vector<pair<Chord, double> >& chords){
	return vector<pair<Note, double> >();
}

vector<pair<Note, double> > generateMelody(){
	Chord tonicChord(getNoteFrequency("A"), minor);
	vector<pair<Chord, double> > chords = generateChords(tonicChord);
	vector<pair<Note, double> > accompanement = generateAccompanement(chords);
	vector<pair<Note, double> > maintheme = generateMaintheme(chords);
	vector<pair<Note, double> > melody;
	int leftIndex = 0, rightIndex = 0;
	while (leftIndex < accompanement.size() || rightIndex < maintheme.size()){
		if (leftIndex == accompanement.size()){
			melody.push_back(maintheme[rightIndex++]);
		} else
		if (rightIndex == maintheme.size()){
			melody.push_back(accompanement[leftIndex++]);
		} else
		if (maintheme[rightIndex].second < accompanement[leftIndex].second){
			melody.push_back(maintheme[rightIndex++]);
		} else {
			melody.push_back(accompanement[leftIndex++]);
		}
	}
	return melody;
}

