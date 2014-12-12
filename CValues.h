#ifndef __CValues__

#define __CValues__

// math constants
const double PI = 3.14159265358979;
const double TWO_PI = 6.2831853071796;

// generator constants
const double MIN_SECONDS_IN_BAR = 2.0;
const double MAX_SECONDS_IN_BAR = 4.0;

// track constants
const int SAMPLE_RATE = 44100;

// instrument constants
const double MAX_FREQUENCY = 10000.0;
const int NUMBER_OF_INSTRUMENTS = 4;

// note constants
const double REAL_FREQUENCY = 220.0;
const double MAX_AMPLITUDE = 100.0;
const double MIN_AMPLITUDE = 0.0;

//maximum and minimum constants of effects
const double ECHO_MAX_DELAY = 1.0;
const double ECHO_MIN_DELAY = 0.1;
const double ECHO_MAX_POWER = 1.0;
const double ECHO_MIN_POWER = 0.2;

const double TREMOLO_MAX_FREQUENCY = 2;
const double TREMOLO_MIN_FREQUENCY = 0.5;
const double TREMOLO_MAX_POWER = 0.3;
const double TREMOLO_MIN_POWER = 0.0;

const double FLANGER_MAX_FREQUENCY = 1.0;
const double FLANGER_MIN_FREQUENCY = 1.0;
const double FLANGER_MAX_DELAY = 0.012;
const double FLANGER_MIN_DELAY = 0.006;
const double FLANGER_MAX_POWER = 0.3;
const double FLANGER_MIN_POWER = 0.0;

const double DISTORTION_MAX_POWER = 1.0;
const double DISTORTION_MIN_POWER = 0.0;
const double DISTORTION_MAX_LEVEL = 1.0;
const double DISTORTION_MIN_LEVEL = 0.1;

#endif
