#include "Motor.h"



MOTOR::MOTOR(int loadPin_, int pwmpin_forward_, int pwmpin_backwards_, int forward_channelNo_, int backwards_channelNo_, LOGGER *logger_) {
  loadPin = loadPin_;
  pwmpin_forward = pwmpin_forward_;
  pwmpin_backwards = pwmpin_backwards_;
  forward_channelNo = forward_channelNo_;
  backwards_channelNo = backwards_channelNo_;
  logger = logger_;
}

void MOTOR::setup() {
  pinMode(pwmpin_forward, OUTPUT);
  ledcAttachPin(pwmpin_forward, forward_channelNo);
  ledcSetup(forward_channelNo, freq, resolution);
  logger->log("Setup motor with forward pin " + String(pwmpin_forward), true);
  logger->log("Setup motor with forward channel " + String(forward_channelNo), true);

  logger->log("PWM pin backwards: " +  String(pwmpin_backwards), true);
  if (pwmpin_backwards > 0) {
    pinMode(pwmpin_backwards, OUTPUT);
    ledcAttachPin(pwmpin_backwards, backwards_channelNo);
    ledcSetup(backwards_channelNo, freq, resolution);
    logger->log("Setup motor with backwards pin " + String(pwmpin_backwards), true);
    logger->log("Setup motor with backwards channel " + String(backwards_channelNo), true);
  }

  pinMode(loadPin, INPUT);
  adcAttachPin(loadPin);
}

int MOTOR::getLoad() {
  return filteredLoad;
}

void MOTOR::doLoop() {
    currentLoadRead = analogRead(loadPin);
    filteredLoad = currentLoadRead 
      - (filteredLoad / 10) 
      + (currentLoadRead / 10);
}

int MOTOR::setSpeed(int targetSpeed, int actionTime) {
		unsigned long _now = millis();
		if (targetSpeed != ot_currentTargetValue) {
			ot_currentTargetValue = targetSpeed;
			ot_startingValue = ot_currentValue;
      ot_actionTime = actionTime;
			ot_setTime = _now;

      //Serial.println("New speed " + String(ot_currentTargetValue));
		}

	if (ot_currentTargetValue == ot_currentValue) {
      //Serial.print("Speed is already set: ");
      //Serial.print(targetSpeed);
      _atTargetSpeed = true;
      return 0;
    }
    else {
      int newValue;

      if (ot_actionTime == 0) {
        //Serial.println("Actiontime zero");

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
      //Serial.println("New value: " + String(newValue));
      if (newValue > 0) {
        ledcWrite(forward_channelNo,abs(newValue));
        ledcWrite(backwards_channelNo,0);
      } else {
        ledcWrite(forward_channelNo,0);
        ledcWrite(backwards_channelNo,abs(newValue));

      }
      
      ot_currentValue = newValue;
      _atTargetSpeed = ot_currentTargetValue == ot_currentValue;
      // if (_atTargetSpeed){
      //   logger->log(logName + String(newValue) + "T" + String(ot_currentTargetValue), true);
      // }      

    }

    bool r = ot_currentTargetValue - ot_currentValue;
    _atTargetSpeed = r == 0;
    return r;
}

bool MOTOR::isAtTargetSpeed() {
  return _atTargetSpeed;;
}


int MOTOR::getSpeed() {
  return ot_currentValue;
}


