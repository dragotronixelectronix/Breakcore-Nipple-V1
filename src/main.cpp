#include <Arduino.h>
#include "DaisyDuino.h"
#include <MIDI.h>

#include <Config.h>



#define BPM 200


void AudioCallback(float **in, float **out, size_t size);

void HandleJoyStick();
void StartRepeat(uint8_t index);
void SamplesPerClock();

void HandleStart();
void HandleClock();
void HandleStop();


MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

//Midi stuff//
bool running = false;
bool startRecived = false;
uint8_t pulseCounter = 0;
bool clockRecived = false;
bool clockCounter = false;


volatile bool clockIntervalReady = false;
volatile uint32_t lastClockSamples = 0;
volatile uint32_t samplesPerClockInstant = 0;
volatile uint32_t sampleCounter = 0;
volatile  uint32_t samplesPerQuarter = 0;
volatile bool quarterReady = false;
float DSY_SDRAM_BSS bufL[MAX_BUFFER];
float DSY_SDRAM_BSS bufR[MAX_BUFFER];

volatile bool repeatRequest = false;

uint32_t writePos = 0;
uint32_t repeatStart = 0;
uint32_t repeatPos = 0;
uint32_t repeatLength = 0;
const uint8_t repeat_slice[] = {4, 8, 16, 32};
volatile uint8_t repeatIndex = 2;
uint8_t activeRepeatIndex = 255;

bool wasRepeating = false;

uint16_t jvalue_x;
uint16_t jvalue_y;


void StartRepeat(uint8_t index)
{
  uint8_t division = repeat_slice[index];

  repeatLength = (SAMPLERATE * 60UL * 4UL) / (BPM * division);

  if (repeatLength > MAX_BUFFER) {
    repeatLength = MAX_BUFFER;
  }

  if (writePos >= repeatLength) {
    repeatStart = writePos - repeatLength;
  } else {
    repeatStart = MAX_BUFFER + writePos - repeatLength;
  }

  repeatPos = 0;
}

void AudioCallback(float **in, float **out, size_t size)
{
  bool repeating = repeatRequest;
  uint8_t wantedRepeatIndex = repeatIndex;

  // Start repeat, or change slice while already repeating
  if (repeating && (!wasRepeating || wantedRepeatIndex != activeRepeatIndex) && (pulseCounter == 0 || pulseCounter == 12))  {
    activeRepeatIndex = wantedRepeatIndex;
    StartRepeat(activeRepeatIndex);
  }

  wasRepeating = repeating;

  for (size_t i = 0; i < size; i++)
  {
    if (repeating)
    {
      uint32_t readPos = repeatStart + repeatPos;

      if (readPos >= MAX_BUFFER) {
        readPos -= MAX_BUFFER;
      }

      out[0][i] = bufL[readPos];
      out[1][i] = bufR[readPos];

      repeatPos++;

      if (repeatPos >= repeatLength) {
        repeatPos = 0;
      }
    }
    else
    {
      out[0][i] = in[0][i];
      out[1][i] = in[1][i];

      bufL[writePos] = in[0][i];
      bufR[writePos] = in[1][i];

      writePos++;

      if (writePos >= MAX_BUFFER) {
        writePos = 0;
      }
    }
    sampleCounter++;
  }

}

void HandleJoyStick()
{
  jvalue_x = analogRead(JOY_X);
  jvalue_y = analogRead(JOY_Y);

  // Dead zone around center
  bool centered =
    jvalue_x >= 400 && jvalue_x <= 600 &&
    jvalue_y >= 400 && jvalue_y <= 600;

  if (centered) {
    repeatRequest = false;
  } else {
    repeatRequest = true;
  }

  if (jvalue_x < 400 && jvalue_y >= 500) {
    repeatIndex = 0;
  }

  if (jvalue_x > 400 && jvalue_y <= 300) {
    repeatIndex = 1;
  }

  if (jvalue_x > 400 && jvalue_y >= 600) {
    repeatIndex = 2;
  }

  if (jvalue_x > 600 && jvalue_y >= 500) {
    repeatIndex = 3;
  }
  



 /*  Serial.print("X: ");
  Serial.print(jvalue_x);
  Serial.print("  Y: ");
  Serial.print(jvalue_y);
  Serial.print("  Repeat: ");
  Serial.println(repeatRequest); */
}

void setup()
{

  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleStart(HandleStart);
  MIDI.setHandleClock(HandleClock);
  MIDI.setHandleStop(HandleStop);
  Serial.begin(115200);

  pinMode(JOY_X, INPUT);
  pinMode(JOY_Y, INPUT);
  pinMode(JOY_BTN, INPUT_PULLUP);

  DAISY.init(DAISY_SEED, AUDIO_SR_48K);
  DAISY.begin(AudioCallback);
}

void loop()
{
  MIDI.read();   
  HandleJoyStick();
  SamplesPerClock();
}

void SamplesPerClock()
{
  if (quarterReady) {
    quarterReady = false;

    Serial.print("samples per quarter: ");
    Serial.println(samplesPerQuarter);

    float bpm = 60.0f * 48000.0f / (float)samplesPerQuarter;

    Serial.print("BPM: ");
    Serial.println(bpm);
  }
}

// MIDI handlers

void HandleStart()
{
  running = true;
  startRecived = true;
  pulseCounter = 0;
  sampleCounter = 0;
}

void HandleClock()
{
  uint32_t now = sampleCounter;

  samplesPerClockInstant = now - lastClockSamples;
  lastClockSamples = now;

  clockIntervalReady = true;

  if (!running) return;

  pulseCounter++;

  if (pulseCounter >= 24) {
    pulseCounter = 0;
    samplesPerQuarter = sampleCounter;
    sampleCounter = 0;
    quarterReady = true;
  }
}

void HandleStop()
{
  running = false;
  clockRecived = false;
}