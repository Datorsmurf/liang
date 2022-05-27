#include "hardwarebutton.h"
#include "Logger.h"



HardwareButton::HardwareButton(int pin_, LOGGER *logger_) {
    pin = pin_;
    logger = logger_;
    pressed = false;
}

bool HardwareButton::IsPressed() {
    return pressed;
}
bool HardwareButton::GetConsumablePress() {
    if (pressed && !pressConsumed) {
        pressConsumed = true;
        return true;
    }
    return false;
}

void HardwareButton::check() {
  if (digitalRead(pin) == LOW) {
      pressedAt = millis();
      pressed = true;
  } 

  else{
      if (pressed && hasTimeout(pressedAt, 20)) {
          pressed = false;
          pressConsumed = false;
          pressedAt = 0;
      }
  }
}

