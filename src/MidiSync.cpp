#include "MidiSync.h"
#include "Config.h"

bool running = false;
bool startRecived = false;
uint8_t pulseCounter = 0;
bool clockRecived = false;

volatile uint32_t totalSamples = 0;

volatile uint32_t lastClockSample = 0;
volatile uint32_t quarterStartSample = 0;

volatile uint32_t samplesPerClockInstant = 0;
volatile uint32_t samplesPerQuarter = 0;

volatile bool quarterReady = false;
volatile bool clockIntervalReady = false;

void HandleStart()
{
  running = true;
  startRecived = true;
  pulseCounter = 0;

  uint32_t now = totalSamples;

  lastClockSample = now;
  quarterStartSample = now;
}

void HandleClock()
{
  clockRecived = true;

  uint32_t now = totalSamples;

  samplesPerClockInstant = now - lastClockSample;
  lastClockSample = now;
  clockIntervalReady = true;

  if (!running) return;

  pulseCounter++;

  if (pulseCounter >= 24) {
    pulseCounter = 0;

    samplesPerQuarter = now - quarterStartSample;
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

    float bpm = 60.0f * 48000.0f / (float)samplesPerQuarter;

    Serial.print("samples per quarter: ");
    Serial.print(samplesPerQuarter);

    Serial.print("  samples per clock: ");
    Serial.print(samplesPerClockInstant);

    Serial.print("  BPM: ");
    Serial.println(bpm);
  }
}