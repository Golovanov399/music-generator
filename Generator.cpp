#include <vector>
#include <cstdlib>
#include <string>
#include <ctime>
#include <map>
#include <algorithm>
#include <iostream>
#include "CValues.h"
#include "Generator.h"
#include "Note.h"

using namespace std;
using namespace generator;

int getCountOfTrailingZeroes(int x){
	if (x)
		return __builtin_ctz(x);
	else
		return -1;
}

template <typename T>
T randElement(const vector<T>& data){
	if (data.size() == 0)
		return T();
	else
		return data[rand() % ((int)data.size())];
}

int generator::getNoteFrequency(const string& noteName, int octave = 0) {
	int currentFrequency = 0;
  // FixMe: do not use switch for this purpose:
  // static const std::map<char, int> note2freq = { { 'C', 0 }, ... };
	switch (noteName[0]){
		case 'C':
			currentFrequency = 0;
			break;
		case 'D':
			currentFrequency = 2;
			break;
		case 'E':
			currentFrequency = 4;
			break;
		case 'F':
			currentFrequency = 5;
			break;
		case 'G':
			currentFrequency = 7;
			break;
		case 'A':
			currentFrequency = 9;
			break;
		case 'B':
			currentFrequency = 10;
			break;
		default:
			currentFrequency = 11;
			break;
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
  // FixMe: do not use switches for this
	//later
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
	int count = 1 << (rand() % 2 + 1);  // FixMe: too complicated	//ur mom is too complicated
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

vector<pair<Note, double> > Generator1::generateAccompaniment(const vector<pair<Chord, double> >& chords, const Chord& tonicChord) const{
	vector<pair<Note, double> > accompaniment;
	for (int i = 0; i < chords.size(); i++){
		for (int j = 1; j <= 5; j += 2){
			accompaniment.push_back(make_pair(getNoteFrequencyByIndex(chords[i].first, j), chords[i].second));
		}
	}

	for (int i = 0; i < accompaniment.size(); i++){
		accompaniment[i].first.setVolume(accompaniment[i].first.getVolume() / 3);
	}

	return accompaniment;
}

vector<pair<Note, double> > Generator1::generateMaintheme(const vector<pair<Chord, double> >& chords, const Chord& tonicChord) const{
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
				Note(getNoteFrequencyByIndex(tonicChord, baseSequence[j].second.getFrequency() + getNoteIndex(tonicChord, chords[i].first.getNote())),
					baseSequence[j].second.getDuration(),
					baseSequence[j].second.getVolume()),
				chords[i].second + baseSequence[j].first));
		}
	}

	return maintheme;
}

vector<pair<Chord, double> > Generator2::generateChords(const Chord& tonicChord) const{
	/* 	Each element of vector<pair<Chord, double>>
		is a pair of a Chord and its appearance time
	*/
	vector<pair<Chord, double> > chords;

	srand(time(NULL));
	
	int tonicFrequency = tonicChord.getNote().getFrequency();
	pair<Mode, int> currentChord(MINOR, 1);
	auto nextChord = [&](){
		if (currentChord.first == MINOR){
			switch ((currentChord.second + 7) % 7){
				case 1:
					currentChord = randElement(vector<pair<Mode, int>>{make_pair(MAJOR, -1), make_pair(MAJOR, 3)});
					break;
				case 2:
					currentChord = randElement(vector<pair<Mode, int>>{});
					break;
				case 3:
					currentChord = randElement(vector<pair<Mode, int>>{});
					break;
				case 4:
					currentChord = randElement(vector<pair<Mode, int>>{make_pair(MAJOR, 5)});
					break;
				case 5:
					currentChord = randElement(vector<pair<Mode, int>>{make_pair(MINOR, 1)});
					break;
				case 6:
					currentChord = randElement(vector<pair<Mode, int>>{});
					break;
				default:
					currentChord = randElement(vector<pair<Mode, int>>{});
					break;
			}
		} else {
			switch ((currentChord.second + 7) % 7){
				case 1:
					currentChord = randElement(vector<pair<Mode, int>>{});
					break;
				case 2:
					currentChord = randElement(vector<pair<Mode, int>>{});
					break;
				case 3:
					currentChord = randElement(vector<pair<Mode, int>>{make_pair(MAJOR, 6), make_pair(MAJOR, 5)});
					break;
				case 4:
					currentChord = randElement(vector<pair<Mode, int>>{});
					break;
				case 5:
					currentChord = randElement(vector<pair<Mode, int>>{make_pair(MINOR, 1)});
					break;
				case 6:
					currentChord = randElement(vector<pair<Mode, int>>{make_pair(MAJOR, 3), make_pair(MAJOR, 7)});
					break;
				default:
					currentChord = randElement(vector<pair<Mode, int>>{make_pair(MAJOR, 3), make_pair(MAJOR, 6)});
					break;
			}
		}
	};

	int countOfChords = 1 << 3;
	for (int i = 0; i < countOfChords - 1; i++){
		chords.push_back(make_pair(Chord(getNoteFrequencyByIndex(tonicChord, currentChord.second),
									currentChord.first), basicChordLength * i));
		nextChord();
	}
	chords.push_back(make_pair(tonicChord, basicChordLength * (countOfChords - 1)));
	
	return chords;
}

vector<pair<Note, double> > Generator2::generateAccompaniment(const vector<pair<Chord, double> >& chords, const Chord& tonicChord) const{
	vector<pair<Note, double> > accompaniment;
	
	for (int i = 0; i < chords.size(); i++){
		accompaniment.push_back(make_pair(Note(getNoteFrequencyByIndex(chords[i].first, 1), basicChordLength / 8), chords[i].second));
		accompaniment.push_back(make_pair(Note(getNoteFrequencyByIndex(chords[i].first, 3), basicChordLength / 8), chords[i].second
																										+ basicChordLength * 0.125));
		accompaniment.push_back(make_pair(Note(getNoteFrequencyByIndex(chords[i].first, 5), basicChordLength / 8), chords[i].second
																										+ basicChordLength * 0.25));
		accompaniment.push_back(make_pair(Note(getNoteFrequencyByIndex(chords[i].first, 3), basicChordLength / 8), chords[i].second
																										+ basicChordLength * 0.375));
		accompaniment.push_back(make_pair(Note(getNoteFrequencyByIndex(chords[i].first, 3), basicChordLength / 8), chords[i].second
																										+ basicChordLength * 0.5));
		accompaniment.push_back(make_pair(Note(getNoteFrequencyByIndex(chords[i].first, 5), basicChordLength / 8), chords[i].second
																										+ basicChordLength * 0.5));
		accompaniment.push_back(make_pair(Note(getNoteFrequencyByIndex(chords[i].first, 8), basicChordLength / 8), chords[i].second
																										+ basicChordLength * 0.5));
		accompaniment.push_back(make_pair(Note(getNoteFrequencyByIndex(chords[i].first, 5), basicChordLength / 8), chords[i].second
																										+ basicChordLength * 0.625));
		accompaniment.push_back(make_pair(Note(getNoteFrequencyByIndex(chords[i].first, 3), basicChordLength / 8), chords[i].second
																										+ basicChordLength * 0.75));
		accompaniment.push_back(make_pair(Note(getNoteFrequencyByIndex(chords[i].first, 5), basicChordLength / 8), chords[i].second
																										+ basicChordLength * 0.875));
	}

	return accompaniment;
}

vector<pair<Note, double> > Generator2::generateMaintheme(const vector<pair<Chord, double> >& chords, const Chord& tonicChord) const{
	vector<pair<Note, double> > maintheme;

	vector<pair<double, int> > baseSequence;
	int count = 1 << 2;
	baseSequence.push_back(make_pair(0, 1));
	for (int i = 1; i < count; i++){
		baseSequence.push_back(make_pair(basicChordLength / count * i, 1 + 2 * (rand() % 3)));
	}
	int countInside = 4;
	for (int i = 0; i < count; i++){
		for (int j = 1; j < countInside; j++){
			baseSequence.push_back(make_pair(basicChordLength / count * (i + 1.0 * j / countInside),
				(baseSequence[i].second * (countInside - j) +
				baseSequence[(i + 1) % count].second * j) / countInside));
		}
	}
	sort(baseSequence.begin(), baseSequence.end());

	for (int i = 0; i < chords.size(); i++){
		for (int j = 0; j < baseSequence.size(); j++){
			maintheme.push_back(make_pair(Note(getNoteFrequencyByIndex(chords[i].first, baseSequence[j].second),
				basicChordLength / count / countInside),
				basicChordLength * i + baseSequence[j].first));
		}
	}

	return maintheme;
}

Melody Generator::generateMelody() const{
	Chord tonicChord(getNoteFrequency("A"), MINOR);
	vector<pair<Chord, double> > chords = generateChords(tonicChord);
	vector<pair<Note, double> > accompaniment = generateAccompaniment(chords, tonicChord);
	vector<pair<Note, double> > maintheme = generateMaintheme(chords, tonicChord);	
	return Melody(accompaniment, maintheme, MIN_SECONDS_IN_BAR + rand() * (MAX_SECONDS_IN_BAR - MIN_SECONDS_IN_BAR) / RAND_MAX);
}
