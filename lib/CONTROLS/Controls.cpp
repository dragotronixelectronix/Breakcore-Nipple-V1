#include <Config.h>
#include <Hardware.h>


//Joystick 1 basics
void HandleJoyStick()
{
  jvalue_x = analogRead(JOY_X);
  jvalue_y = analogRead(JOY_Y);

  // Dead zone around center
  bool centered =
    jvalue_x >= 400 && jvalue_x <= 600 &&
    jvalue_y >= 400 && jvalue_y <= 600;

  if (centered) {
    joyOn = false;
  } else {
    joyOn = true;
  }

  if (jvalue_x < 400 && jvalue_y >= 500) {
    joyArea = 0;
  }

  if (jvalue_x > 400 && jvalue_y <= 300) {
    joyArea = 1;
  }

  if (jvalue_x > 400 && jvalue_y >= 600) {
    joyArea = 2;
  }

  if (jvalue_x > 600 && jvalue_y >= 500) {
    joyArea = 3;
  }
}