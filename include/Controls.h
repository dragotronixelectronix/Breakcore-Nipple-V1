#pragma once

#include <Arduino.h>

extern float jvalue_x;
extern float jvalue_y;
extern bool jbtn;
extern bool centered;

extern volatile bool joyOn;
extern volatile uint8_t joyArea;

void InitControls();
void HandleJoyStick();