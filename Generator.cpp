#include <vector>
#include <cstdlib>
#include <string>
#include <ctime>
#include "Generator.h"
#include "Note.h"

using namespace std;
using namespace generator;

int getCountOfTrailingZeroes(int x){
	//to be replaced by __builtin_
	if (!x)
		return -1;
	int count = 0;
	while (!(x & 1)){
		count++;
		x >>= 1;
	}
	return count;
}

int generator::getNoteFrequency(const string& noteName, int octave = 0) {
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

Chord generator::generateTonicChord(){
	return Chord(Note(getNoteFrequency("A")), MINOR);
}

int generator::getNoteFrequencyByIndex(const Chord& tonicChord, int index){
	// 1-indexation here
	int octave = 0;
	while (index < 1){
		octave--;
		index += 7;
	}
	while (index > 7){
		octave++;
		index -= 7;
	}
	int baseFrequency = tonicChord.getNote().getFrequency() + octave * 12;
	if (tonicChord.getMode() == MAJOR){
		switch(index){
			case 1:
				return baseFrequency + 0;
			case 2:
				return baseFrequency + 2;
			case 3:
				return baseFrequency + 4;
			case 4:
				return baseFrequency + 5;
			case 5:
				return baseFrequency + 7;
			case 6:
				return baseFrequency + 9;
			case 7:
				return baseFrequency + 11;
			default:
				return baseFrequency + 12;
		}
	} else {
		switch(index){
			case 1:
				return baseFrequency + 0;
			case 2:
				return baseFrequency + 2;
			case 3:
				return baseFrequency + 3;
			case 4:
				return baseFrequency + 5;
			case 5:
				return baseFrequency + 7;
			case 6:
				return baseFrequency + 8;
			case 7:
				return baseFrequency + 10;
			default:
				return baseFrequency + 12;
		}		
	}
}

int generator::getNoteIndex(const Chord& tonicChord, const Note& note){
	for (int i = 1; i <= 7; i++){
		if ((getNoteFrequencyByIndex(tonicChord, i) - note.getFrequency()) % 12 == 0){
			return i + ((note.getFrequency() - getNoteFrequencyByIndex(tonicChord, i)) / 12) * 7;
		}
	}
	return 1;
}

int generator::getNextNoteFrequency(const Chord& tonicChord, const Note& note, int additor){
	int noteIndex = getNoteIndex(tonicChord, note);
	noteIndex += additor;
	return getNoteFrequencyByIndex(tonicChord, noteIndex);
}

Generator::Generator(){
	basicChordLength = 1;
}

vector<pair<Chord, double> > Generator1::generateChords(const Chord& tonicChord) const{
	/* 	Each element of vector<pair<Chord, double>>
		is a pair of a Chord and its appearance time
	*/
	vector<pair<Chord, double> > chords;
	int tonicFrequency = tonicChord.getNote().getFrequency();
	srand(time(NULL));
	int count = 1 << (rand() % 2 + 1);
	double chordLength = basicChordLength;
	vector<Chord> combo;
	combo.push_back(Chord(getNoteFrequencyByIndex(tonicChord, 1), MINOR));
	combo.push_back(Chord(getNoteFrequencyByIndex(tonicChord, -1), MAJOR));
	combo.push_back(Chord(getNoteFrequencyByIndex(tonicChord, 3), MAJOR));
	combo.push_back(Chord(getNoteFrequencyByIndex(tonicChord, 7), MAJOR));
	for (int i = 0; i < count; i++){
		for (int j = 0; j < combo.size(); j++){
			chords.push_back(make_pair(combo[j], (combo.size() * i + j) * chordLength));
		}
	}
	return chords;
}

vector<pair<Note, double> > Generator1::generateAccompanement(const vector<pair<Chord, double> >& chords) const{
	vector<pair<Note, double> > accompanement;
	for (int i = 0; i < chords.size(); i++){
		for (int j = 1; j <= 5; j += 2){
			accompanement.push_back(make_pair(getNoteFrequencyByIndex(chords[i].first, j), chords[i].second));
		}
	}

	for (int i = 0; i < accompanement.size(); i++){
		accompanement[i].first.setVolume(accompanement[i].first.getVolume() / 3);
	}

	return accompanement;
}

vector<pair<Note, double> > Generator1::generateMaintheme(const vector<pair<Chord, double> >& chords) const{
	vector<pair<Note, double> > maintheme;
	double baseVolume = MAX_AMPLITUDE / 4;
	double guaranteedVolume = baseVolume * 0.7;
	vector<pair<double, Note> > baseSequence;
	int sampleDegree = 3;
	baseSequence.resize(1 << sampleDegree);
	for (int i = 0; i < baseSequence.size(); i++){
		if (i > 0)
			baseSequence[i] = make_pair(basicChordLength * i / baseSequence.size(),	//all is ok, don't worry
										Note((1 << (sampleDegree - 1)) - abs((1 << (sampleDegree - 1)) - i),
											1.0 * basicChordLength / (1 << sampleDegree), 
											guaranteedVolume + (baseVolume - guaranteedVolume) *
											getCountOfTrailingZeroes(i) / sampleDegree));
		else
			baseSequence[i] = make_pair(basicChordLength * i / baseSequence.size(),
										Note((1 << (sampleDegree - 1)) - abs((1 << (sampleDegree - 1)) - i),
											1.0 * basicChordLength / (1 << sampleDegree), 
											baseVolume));
	}
	for (int i = 0; i < chords.size(); i++){
		for (int j = 0; j < baseSequence.size(); j++){
			maintheme.push_back(make_pair(
				Note(getNoteFrequencyByIndex(chords[i].first, baseSequence[j].second.getFrequency() + 1),
					baseSequence[j].second.getDuration(),
					baseSequence[j].second.getVolume()),
				chords[i].second + baseSequence[j].first));
		}
	}

	return maintheme;
}

vector<pair<Note, double> > Generator::generateMelody() const{
	Chord tonicChord(getNoteFrequency("A"), MINOR);
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

