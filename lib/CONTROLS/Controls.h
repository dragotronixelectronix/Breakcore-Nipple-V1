#include <Arduino.h>
#pragma once


//Joystick 1
uint16_t jvalue_x;
uint16_t jvalue_y;
bool jbtn;
bool centered;

volatile bool joyOn = false;
volatile uint8_t joyArea = 2;