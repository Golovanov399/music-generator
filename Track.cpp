#include <cassert>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include "Note.h"
#include "Instrument.h"
#include "Track.h"
#include "CValues.h"

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

Track::Track(const Note& element, const Instrument& instrument,  double secondsInBar, double phase)
{
	double duration = element.getDuration() * secondsInBar;
	int waveSize = instrument.getRealDuration(duration) * (double) SAMPLE_RATE;
	double frequency = (TWO_PI * element.getRealFrequency()) / (double) SAMPLE_RATE;
	double volume = element.getVolume();
	int time = 0; // time in seconds/SAMPLE_RATE
	for (int time = 0; time <= waveSize; ++time)
		wave_.push_back(instrument.getWaveValue(frequency, phase, volume, time, duration));
}

Track::Track(const noteSequence& sequence, const Instrument& instrument, double secondsInBar) // naive constructor;
{
	wave_.clear();
	for (int i = 0; i < (int) sequence.size(); i++)
	{
		int offset = (int) (sequence[i].second * secondsInBar * SAMPLE_RATE);
		double phase = TWO_PI * sequence[i].first.getRealFrequency() * (sequence[i].second * secondsInBar);
		addToSelf((int) wave_.size() - offset, Track(sequence[i].first, instrument, secondsInBar, phase));
	}
}

Track::Track(const Melody& melody, const Instrument& accompanimentInstrument, const Instrument& mainInstrument)
{
	wave_.clear();
	addToSelf(0, Track(melody.getAccompaniment(), accompanimentInstrument, melody.getSecondsInBar()));
	addToSelf((int) wave_.size(), Track(melody.getMainTheme(), mainInstrument, melody.getSecondsInBar()));
}

int Track::getLength() const
{
	return (int) wave_.size();
}

double Track::getValue(const size_t &index) const
{
	assert(index < wave_.size());
	return wave_[index];
}

void Track::modifyValue(const size_t &index,
			const double &value)
{
	assert(index < wave_.size());
	wave_[index] += value;
}

Track Track::add(int offset, const Track& delta) const
{
	assert(offset <= (int) wave_.size());
	std::vector<double> result(std::max((int) wave_.size(), (int) wave_.size() + delta.getLength() - offset), 0.0);
	for (int i = 0; i < (int) result.size(); i++)
	{
		result[i] = (i < (int) wave_.size() ? wave_[i] : 0.0);
		if (i >= (int) wave_.size() - offset && i < (int)(wave_.size() + delta.getLength()) - offset)
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
	for (int i = OldWaveSize - offset; i < OldWaveSize - offset + (int)delta.getLength(); ++i)
		wave_[i] += delta.getValue(i - (OldWaveSize - offset));
}

void Track::normalize()
{
	double mx = 0;
	for (int i = 0; i < (int)wave_.size(); i++)
		if (fabs(wave_[i]) > mx) mx = fabs(wave_[i]);
	std::cerr << "Maximum absolute value : " << mx << std::endl;
	//if (mx > MAX_AMPLITUDE - 1)
	for (int i = 0; i < (int)wave_.size(); i++)
		wave_[i] *= ((MAX_AMPLITUDE - 1)/ mx);
}

void Track::drop(const char* outputFileName) const
{
	int	chunk_id = 0x46464952; // RIFF
	int	chunk_size;
	int	format = 0x45564157; // WAVE
	int	subchunk1_id = 0x20746d66; // "fmt "
	int	subchunk1_size = 16; // for PCM
	short	audio_format = 1;
	short	num_channels = 1;
	short	bits_per_sample = 16;
	int	sample_rate = SAMPLE_RATE;
	int	byte_rate = sample_rate * num_channels * bits_per_sample / 8;
	short	block_align = num_channels * bits_per_sample / 8;
	int	subchunk2_id = 0x61746164; // data
	int	subchunk2_size = wave_.size() * num_channels * bits_per_sample / 8;
		chunk_size = 4 + (8 + subchunk1_size) + (8 + subchunk2_size);

	FILE* p_file = fopen(outputFileName, "wb");

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
	for (int i = 0; i < (int) wave_.size(); i++)
	{
		short v = (short) (wave_[i] * 32767 / MAX_AMPLITUDE);
		fwrite(&v, sizeof(short), 1, p_file);
	}

	fclose(p_file);
}

void Track::resize(const size_t &size)
{
	wave_.resize(size);
}
