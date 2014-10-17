#include <cmath>	//for pow(x, y)
#include "Note.h"

Note::Note()
{
  frequency_ = 440;
  duration_ = 1;
  volume_ = MAX_AMPLITUDE / 4;
}

Note::Note(int frequency, double duration = 1, double volume = MAX_AMPLITUDE / 4)
{
  frequency_ = frequency;
  duration_ = duration;
  volume_ = volume;
}

int Note::getFrequency() const
{
  return frequency_;
}

double Note::getRealFrequency() const
{
  return pow(2, 1.0 * (frequency_ - 9) / 12) * REAL_FREQUENCY;
}

double Note::getDuration() const
{
  return duration_;
}

double Note::getVolume() const
{
  return volume_;
}

Chord::Chord(Note note, Mode mode)
{
  note_ = note;
  mode_ = mode;
}

Mode Chord::getMode() const
{
  return mode_;
}
	
Note Chord::getNote() const
{
  return note_;
}

int Chord::getFrequency() const
{
  return note_.getFrequency();
}

double Chord::getDuration() const
{
  return note_.getDuration();
}
	
double Chord::getVolume() const
{
  return note_.getVolume();
}
