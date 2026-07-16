#include <Hardware.h>

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
}