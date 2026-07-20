#pragma once

#include "Config.h"
#include <Arduino.h>
#include <math.h>


//Midi stuff//
extern bool running;
extern bool startRecived;
extern uint8_t pulseCounter;
extern bool clockRecived;
extern bool clockCounter;

extern volatile bool clockIntervalReady;
extern volatile float lastClockSamples;
extern volatile float samplesPerClockInstant;
extern volatile float sampleCounter;
extern volatile  float rawsamplesPerQuarter;
extern volatile  float samplesPerQuarter;
extern volatile bool quarterReady;
extern volatile uint16_t bpm;


void HandleStart();
void HandleClock();
void HandleStop();
void SamplesPerClock();
void CountAudioSamples(size_t size);