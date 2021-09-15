
#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <Logger.h>

class MOTOR {
  public:
    MOTOR(int pwmpin_, int dirpin_, int channelNo_, LOGGER *logger_);
    MOTOR(int pwmpin_, int channelNo_, LOGGER *logger_);

	  int setSpeed(int speed, int actionTime);
    int getSpeed();

    void doLoop();
    void setup();

    bool isAtTargetSpeed();

  private:
    int pwmpin;
    int dirpin;
    int speed;
    int channelNo;



    unsigned long ot_setTime = 0;
	  int ot_currentTargetValue = 0;
	  int ot_startingValue = 0;
	  int ot_currentValue = 0;
	  int ot_actionTime = 0;
    bool _atTargetSpeed;

    const int freq = 5000;
    const int resolution = 8;
  LOGGER* logger;

};

#endif /* _MOTOR_H_ */
