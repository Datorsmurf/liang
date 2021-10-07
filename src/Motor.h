
#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <Logger.h>
#include "definitions.h"

#define MOTOR_LOAD_FILTER 10;

class MOTOR {
  public:
    MOTOR(int loadPin_, int pwmpin_forward_, int pwmpin_backwards_, int forward_channelNo_, int backwards_channelNo_, int loadLimit_, LOGGER *logger_);

	  int setSpeed(int speed, int actionTime);
    int getSpeed();

    int getLoad();
    bool isOverload();

    void doLoop();
    void setup();

    bool isAtTargetSpeed();

  private:
    int loadPin;
    int pwmpin_forward;
    int pwmpin_backwards;
    int speed;
  int forward_channelNo;
  int backwards_channelNo;
    int filteredLoad = 0;
    int currentLoadRead = 0;
    int loadLimit;

    unsigned long ot_setTime = 0;
	  int ot_currentTargetValue = -1;
	  int ot_startingValue = 0;
	  int ot_currentValue = -2;
	  int ot_actionTime = 0;
    bool _atTargetSpeed;

    const int freq = 1000;
    const int resolution = 8;
  LOGGER* logger;

};

#endif /* _MOTOR_H_ */
