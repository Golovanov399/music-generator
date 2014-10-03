#include "Track.h"
#include "Note.h"
#include <cassert>
#include <cstdio>

Track::Track()
{
	wave_.clear();
}

Track::Track(const std::vector<double>& wave)
{
	wave_.resize(wave.size());
	for (int i = 0; i < wave.size(); i++)
		wave_[i] = wave[i];
}

//Track::Track(const std::vector<std::pair(Note, double> >& sequence) {} -ToDo

int Track::getLength() const
{
	return wave_.size();
}

double Track::getValue(int i) const
{
	assert(0 <= i && i < (int)wave_.size());
	return wave_[i];
}

Track Track::add(int offset, const Track& delta) const
{
	assert(offset <= (int)wave_.size());
	std::vector<double> result;
	result.resize(wave_.size() + delta.getLength() - offset);
	for (int i = 0; i < (int)result.size(); i++)
	{
		result[i] = (i < (int)wave_.size() ? wave_[i] : 0.0);
		if (i >= wave_.size() - offset)
			result[i] += delta.getValue(i - (wave_.size() - offset));
	}
	return Track(result);
}

void Track::drop() const
{
	FILE* pFile = fopen("sample.wav", "wb");
	int	ChunkID = 0x46464952, // RIFF
		ChunkSize,
		Format = 0x45564157, // WAVE
		Subchunk1ID = 0x20746d66, // "fmt "
		Subchunk1Size = 16; // for PCM
	short	AudioFormat = 1,
		NumChannels = 1,
		BitsPerSample = 16;
	int	SampleRate = SAMPLE_RATE,
		ByteRate = SampleRate * NumChannels * BitsPerSample / 8;
	short	BlockAlign = NumChannels * BitsPerSample / 8;
	int	Subchunk2ID = 0x61746164, // data
		Subchunk2Size = wave_.size() * NumChannels * BitsPerSample / 8;
		ChunkSize = 4 + (8 + Subchunk1Size) + (8 + Subchunk2Size);

	fwrite(&ChunkID, sizeof(int), 1, pFile);	
	fwrite(&ChunkSize, sizeof(int), 1, pFile);
	fwrite(&Format, sizeof(int), 1, pFile);
	fwrite(&Subchunk1ID, sizeof(int), 1, pFile);
	fwrite(&Subchunk1Size, sizeof(int), 1, pFile);
	fwrite(&AudioFormat, sizeof(short), 1, pFile);
	fwrite(&NumChannels, sizeof(short), 1, pFile);
	fwrite(&SampleRate, sizeof(int), 1, pFile);
	fwrite(&ByteRate, sizeof(int), 1, pFile);
	fwrite(&BlockAlign, sizeof(short), 1, pFile);
	fwrite(&BitsPerSample, sizeof(short), 1, pFile);
	fwrite(&Subchunk2ID, sizeof(int), 1, pFile);
	fwrite(&Subchunk2Size, sizeof(int), 1, pFile);


	short Data[wave_.size()];
	for (int i = 0; i < (int)wave_.size(); i++)
		Data[i] = (short)(wave_[i] * 32767 / MAX_AMPLITUDE);
	fwrite(Data, sizeof(short), wave_.size(), pFile);
	fclose(pFile);
}
