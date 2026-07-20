#include "MidiSync.h"
#include "Config.h"
#include <math.h>
bool running = false;
bool startRecived = false;
uint8_t pulseCounter = 0;
bool clockRecived = false;

volatile float totalSamples = 0;

volatile float lastClockSample = 0;
volatile float quarterStartSample = 0;

volatile float samplesPerClockInstant = 0;
volatile float rawsamplesPerQuarter = 0;
volatile float samplesPerQuarter = 0;

volatile bool quarterReady = false;
volatile bool clockIntervalReady = false;

volatile uint16_t bpm;



void HandleStart()
{
  running = true;
  startRecived = true;
  pulseCounter = 0;

  float now = totalSamples;

  lastClockSample = now;
  quarterStartSample = now;
}

void HandleClock()
{
  clockRecived = true;

  float now = totalSamples;

  samplesPerClockInstant = now - lastClockSample;
  lastClockSample = now;
  clockIntervalReady = true;

  if (!running) return;

  pulseCounter++;

  if (pulseCounter > 24) {
    pulseCounter = 1;

    rawsamplesPerQuarter = now - quarterStartSample;
    quarterStartSample = now;

    quarterReady = true;

    

  }
  

  
}

void HandleStop()
{
  running = false;
}

void CountAudioSamples(size_t size)
{
  totalSamples += size;
}

void SamplesPerClock()
{
  if (quarterReady) {
    quarterReady = false;

    //works okay, but needs a buffer to average out the samples per quarter more accuratley
    //array needs to be configured so it responds fast enough to tempo change

    samplesPerQuarter = round((samplesPerQuarter * 0.95f) + (rawsamplesPerQuarter * 0.05f));

    float unrounded = 60.0f * 48000.0f / (float)samplesPerQuarter;
    bpm = round(unrounded);

    Serial.print("samples per quarter: ");
    Serial.print(samplesPerQuarter);

    Serial.print("  samples per clock: ");
    Serial.print(samplesPerClockInstant);

    Serial.print("  BPM: ");
    Serial.println(bpm);
  }
}