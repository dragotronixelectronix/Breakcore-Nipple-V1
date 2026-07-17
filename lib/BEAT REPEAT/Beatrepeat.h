#pragma once
#include <Arduino.h>
#include <Config.h>
#include <DaisyDuino.h>

//Circular buffer into sdram (64mb)
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