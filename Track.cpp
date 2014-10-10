#include "Track.h"
#include "Note.h"
#include <cassert>
#include <cstdio>
#include <cmath>

const int SAMPLE_RATE = 44100;
const double SECONDS_IN_BAR = 1.0;

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

Track::Track(const Note& element)
{
	wave_.resize((int) (element.getDuration() * SECONDS_IN_BAR * SAMPLE_RATE));
	double frequency = element.getRealFrequency();
	frequency = (M_PI * 2.0 * frequency) / SAMPLE_RATE;
	for (int i = 0; i < (int)wave_.size(); i++)
		wave_[i] = element.getVolume() * sin(frequency * i);
}

Track::Track(const std::vector<std::pair<Note, double> >& sequence) // naive constructor;
{
	wave_.clear();
	for (int i = 0; i < (int)sequence.size(); i++)
	{
		int offset = (int) (sequence[i].second * SECONDS_IN_BAR * SAMPLE_RATE);
		addToSelf((int) wave_.size() - offset, Track(sequence[i].first));
	}
}

int Track::getLength() const
{
	return wave_.size();
}

double Track::getValue(int index) const
{
	assert(0 <= index && index < (int) wave_.size());
	return wave_[index];
}

Track Track::add(int offset, const Track& delta) const
{
	assert(offset <= (int) wave_.size());
	std::vector<double> result(std::min((int) wave_.size(), (int) wave_.size() + delta.getLength() - offset), 0.0);
	for (int i = 0; i < (int) result.size(); i++)
	{
		result[i] = (i < (int) wave_.size() ? wave_[i] : 0.0);
		if (i >= (int) wave_.size() - offset)
			result[i] += delta.getValue(i - ((int) wave_.size() - offset));
	}
	return Track(result);
}

void Track::addToSelf(int offset, const Track& delta)
{
	assert(offset <= (int) wave_.size());
	int OldWaveSize = (int) wave_.size();
	if (delta.getLength() - offset > 0)
		wave_.resize((int) wave_.size() + delta.getLength() - offset);
	for (int i = OldWaveSize; i < (int) wave_.size(); ++i)
		wave_[i] = 0;
	for (int i = OldWaveSize - offset; i < (int) wave_.size(); ++i)
		wave_[i] += delta.getValue(i - (OldWaveSize - offset));
}

void Track::drop() const
{
	int	chunk_id = 0x46464952, // RIFF
		chunk_size,
		format = 0x45564157, // WAVE
		subchunk1_id = 0x20746d66, // "fmt "
		subchunk1_size = 16; // for PCM
	short	audio_format = 1,
		num_channels = 1,
		bits_per_sample = 16;
	int	sample_rate = SAMPLE_RATE,
		byte_rate = sample_rate * num_channels * bits_per_sample / 8;
	short	block_align = num_channels * bits_per_sample / 8;
	int	subchunk2_id = 0x61746164, // data
		subchunk2_size = wave_.size() * num_channels * bits_per_sample / 8;
		chunk_size = 4 + (8 + subchunk1_size) + (8 + subchunk2_size);

	FILE* p_file = fopen("sample.wav", "wb");

	fwrite(&chunk_id, sizeof(int), 1, p_file);	
	fwrite(&chunk_size, sizeof(int), 1, p_file);
	fwrite(&format, sizeof(int), 1, p_file);
	fwrite(&subchunk1_id, sizeof(int), 1, p_file);
	fwrite(&subchunk1_size, sizeof(int), 1, p_file);
	fwrite(&audio_format, sizeof(short), 1, p_file);
	fwrite(&num_channels, sizeof(short), 1, p_file);
	fwrite(&sample_rate, sizeof(int), 1, p_file);
	fwrite(&byte_rate, sizeof(int), 1, p_file);
	fwrite(&block_align, sizeof(short), 1, p_file);
	fwrite(&bits_per_sample, sizeof(short), 1, p_file);
	fwrite(&subchunk2_id, sizeof(int), 1, p_file);
	fwrite(&subchunk2_size, sizeof(int), 1, p_file);


	short data[wave_.size()];
	for (int i = 0; i < (int)wave_.size(); i++)
		data[i] = (short)(wave_[i] * 32767 / MAX_AMPLITUDE);
	fwrite(data, sizeof(short), wave_.size(), p_file);
	fclose(p_file);
}
