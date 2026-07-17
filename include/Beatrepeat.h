#pragma once
#include <Arduino.h>
#include "Config.h"
#include "DaisyDuino.h"
#include "AudioBuffer.h"


volatile bool repeatRequest;

uint32_t writePos;
uint32_t repeatStart;
uint32_t repeatPos;
uint32_t repeatLength;
extern const uint8_t repeat_slice[5];
extern volatile uint8_t repeatIndex;
uint8_t activeRepeatIndex;

bool wasRepeating;