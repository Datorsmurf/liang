#include "Motor.h"


MOTOR::MOTOR(int pwmpin_, int channelNo_, LOGGER *logger_) {
  pwmpin = pwmpin_;
  dirpin = 0;
  channelNo = channelNo_;
  logger = logger_;
}


MOTOR::MOTOR(int pwmpin_, int dirpin_, int channelNo_, LOGGER *logger_) {
  pwmpin = pwmpin_;
  dirpin = dirpin_;
  channelNo = channelNo_;
  logger = logger_;
}

void MOTOR::setup() {
  pinMode(pwmpin, OUTPUT);
  if (dirpin != 0) {
    pinMode(dirpin, OUTPUT);
    digitalWrite(dirpin, HIGH);
  }
  ledcSetup(channelNo, freq, resolution);
  ledcAttachPin(pwmpin, channelNo);
}

void MOTOR::doLoop() {
    unsigned long _now = millis();

		if (ot_currentTargetValue == ot_currentValue) {
      //Serial.print("Speed is already set: ");
      //Serial.print(targetSpeed);
      _atTargetSpeed = true;
      return;
    }
    else {
      int newValue;

      if (ot_actionTime == 0) {
        //Serial.print("Actiontime zero");

        newValue = ot_currentTargetValue;
      }
      else {
        if (ot_setTime + ot_actionTime < _now) {
          //Serial.println("Overdue");
          newValue = ot_currentTargetValue;
        }
        else {

          newValue = map(_now, ot_setTime, ot_setTime + ot_actionTime, ot_startingValue, ot_currentTargetValue);
          //Serial.print("Mapping");
          //Serial.print(" ot_startingValue: ");
          //Serial.print(ot_startingValue);
          //Serial.print(" targetSpeed: ");
          //Serial.print(targetSpeed);
        }
      }
      ledcWrite(channelNo, abs(newValue));
      if (dirpin > 0) {
        digitalWrite(dirpin, newValue >= 0);
      }
      ot_currentValue = newValue;
      _atTargetSpeed = ot_currentTargetValue == ot_currentValue;
      // if (_atTargetSpeed){
      //   logger->log(logName + String(newValue) + "T" + String(ot_currentTargetValue), true);
      // }      

    }
    //Serial.print(" newValue: ");
    //Serial.println(newValue);

}

int MOTOR::setSpeed(int targetSpeed, int actionTime) {
		unsigned long _now = millis();
		if (targetSpeed != ot_currentTargetValue) {
			ot_currentTargetValue = targetSpeed;
			ot_startingValue = ot_currentValue;
      ot_actionTime = actionTime;
			ot_setTime = _now;

      //logger->log(logName + String(ot_currentTargetValue));

		}

    bool r = ot_currentTargetValue - targetSpeed;
    _atTargetSpeed = r == 0;
    return r;
}

bool MOTOR::isAtTargetSpeed() {
  return _atTargetSpeed;;
}


int MOTOR::getSpeed() {
  return ot_currentValue;
}


