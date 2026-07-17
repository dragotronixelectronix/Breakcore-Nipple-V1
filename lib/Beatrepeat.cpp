#include "Beatrepeat.h"
#include <AudioBuffer.h>
#include "MidiSync.h"

float DSY_SDRAM_BSS bufL[MAX_BUFFER];
float DSY_SDRAM_BSS bufR[MAX_BUFFER];

volatile bool repeatRequest = false;

uint32_t writePos = 0;
uint32_t repeatStart = 0;
uint32_t repeatPos = 0;
uint32_t repeatLength = 0;
const uint8_t repeat_slice[5] = {4, 8, 16, 32, 64};
volatile uint8_t repeatIndex = 2;
uint8_t activeRepeatIndex = 255;

bool wasRepeating = false;

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

/*  bool repeating = repeatRequest;
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
  } */