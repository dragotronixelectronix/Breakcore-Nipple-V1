#pragma once

#include "Config.h"
#include <Arduino.h>


//Midi stuff//
extern bool running;
extern bool startRecived;
extern uint8_t pulseCounter;
extern bool clockRecived;
extern bool clockCounter;

extern volatile bool clockIntervalReady;
extern volatile uint32_t lastClockSamples;
extern volatile uint32_t samplesPerClockInstant;
extern volatile uint32_t sampleCounter;
extern volatile  uint32_t samplesPerQuarter;
extern volatile bool quarterReady;

void HandleStart();
void HandleClock();
void HandleStop();
void SamplesPerClock();
void CountAudioSamples(size_t size);