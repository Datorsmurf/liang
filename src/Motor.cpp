#include "Motor.h"



MOTOR::MOTOR(int loadPin_, int pwmpin_forward_, int pwmpin_backwards_, int forward_channelNo_, int backwards_channelNo_, int loadLimit_, int ignoreStartLoadsFor_, LOGGER *logger_, const String &logName_) {
  loadPin = loadPin_;
  pwmpin_forward = pwmpin_forward_;
  pwmpin_backwards = pwmpin_backwards_;
  forward_channelNo = forward_channelNo_;
  backwards_channelNo = backwards_channelNo_;
  loadLimit = loadLimit_;
  logger = logger_;
  ignoreStartLoadsFor = ignoreStartLoadsFor_;
  logName = logName_;
}

void MOTOR::setup() {
  pinMode(pwmpin_forward, OUTPUT);
  ledcAttachPin(pwmpin_forward, forward_channelNo);
  ledcSetup(forward_channelNo, freq, resolution);
  if (pwmpin_backwards > 0) {
    pinMode(pwmpin_backwards, OUTPUT);
    ledcAttachPin(pwmpin_backwards, backwards_channelNo);
    ledcSetup(backwards_channelNo, freq, resolution);
  }

  pinMode(loadPin, ANALOG);
  adcAttachPin(loadPin);
}

int MOTOR::getLoad() {
  return filteredLoad;
}

bool MOTOR::isOverload() {
  if(!isAtTargetSpeed()) return false;

  if (!hasTimeout(_atTargetSpeedSince, ignoreStartLoadsFor)) return false;

  //if(getSpeed() < FULL_SPEED * 0.85) return false;
  

  //Handle cut off that in some situations gives zero load. Might trigger 
  if (filteredLoad < 10 && (getSpeed() > 110) && isAtTargetSpeed()) {
    if (_unexpectedLowLoadSince == 0) {
      _unexpectedLowLoadSince = millis();
    } else {
      if (hasTimeout(_unexpectedLowLoadSince, 10000)) {
        _unexpectedLowLoadSince = 0;
        logger->log(logName +  ": Unexpected low load: " + String(filteredLoad) + " Speed: " + String(getSpeed()));
        return true;
      }
    }
  } else {
    _unexpectedLowLoadSince = 0;
  }

  if(filteredLoad > loadLimit) {
    logger->log(logName +  ": Overload: " + String(filteredLoad) + " Speed: " + String(getSpeed()));
    return true;
  };
  return false;
}

void MOTOR::doLoop() {
    currentLoadRead = analogRead(loadPin);
    filteredLoad = filteredLoad * (1-LOAD_FILTER) + currentLoadRead * LOAD_FILTER;
}

int MOTOR::setSpeed(int targetSpeed, int actionTime) {
		unsigned long _now = millis();
		if (targetSpeed != ot_currentTargetValue) {
			ot_currentTargetValue = targetSpeed;
			ot_startingValue = ot_currentValue;
      ot_actionTime = actionTime;
			ot_setTime = _now;

      //logger->log("New speed " + String(ot_currentTargetValue));
		}

	if (ot_currentTargetValue == ot_currentValue) {
      //Serial.print("Speed is already set: ");
      //Serial.print(targetSpeed);
      setAtTargetSpeed(true);
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
    }

    int diff = ot_currentTargetValue - ot_currentValue;
    setAtTargetSpeed(diff == 0);

    return abs(diff);
}

void MOTOR::setAtTargetSpeed(bool value) {
    if (value && !_atTargetSpeed) {
        _atTargetSpeedSince = millis();
        //logger->log(logName + " AtTarget: " + String(ot_currentValue) + " since:" + String(_atTargetSpeedSince));
    }

    _atTargetSpeed = value;
  

}
bool MOTOR::isAtTargetSpeed() {
  return _atTargetSpeed;;
}


int MOTOR::getSpeed() {
  return ot_currentValue;
}


