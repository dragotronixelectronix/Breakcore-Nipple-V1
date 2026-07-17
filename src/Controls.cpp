#include <Arduino.h>
#include "Config.h"
#include "Controls.h"

// Actual variable definitions live here
float jvalue_x = 0;
float jvalue_y = 0;
bool jbtn = false;
bool centered = false;

volatile bool joyOn = false;
volatile uint8_t joyArea = 2;

void InitControls()
{
  pinMode(JOY_X, INPUT);
  pinMode(JOY_Y, INPUT);
  pinMode(JOY_BTN, INPUT_PULLUP);
}

void HandleJoyStick()
{
  jvalue_x = analogRead(JOY_X);
  jvalue_y = analogRead(JOY_Y);
  jbtn = digitalRead(JOY_BTN) == LOW;

  centered =
    jvalue_x >= 400 && jvalue_x <= 600 &&
    jvalue_y >= 400 && jvalue_y <= 600;

  joyOn = !centered;

  if (joyOn) {
    if (jvalue_x < 400 && jvalue_y >= 500) {
      joyArea = 0;
    }
    else if (jvalue_x > 400 && jvalue_y <= 300) {
      joyArea = 1;
    }
    else if (jvalue_x > 400 && jvalue_y >= 600 && jvalue_x <= 600) {
      joyArea = 2;
    }
    else if (jvalue_x > 600 && jvalue_y >= 500) {
      joyArea = 3;
    }
  }
}