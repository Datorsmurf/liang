#ifndef _BEHAVIOUR_MOTOR_DEBUG_H_
#define _BEHAVIOUR_MOTOR_DEBUG_H_

#include "behavior.h"
#include "Controller.h"
#include "Logger.h"
#include "battery.h"
#include "sensor.h"
#include "definitions.h"


class MotorDebug : public BEHAVIOR { 
    public:
        MotorDebug(Controller *controller_, LOGGER *logger_, BATTERY *battery_, SENSOR *leftSensor_, SENSOR *rightSensor_);
        void start();
        int loop();
        int id();
        String desc();
    private:
        Controller *controller;
        LOGGER *logger;
        BATTERY *battery;
        SENSOR *leftSensor;
        SENSOR *rightSensor;
        int currentStep = 0;
        unsigned long stepStart = 0;
        int turnAngle;
        int turnCount = 0;
};

#endif