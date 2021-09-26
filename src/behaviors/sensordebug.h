#ifndef _BEHAVIOUR_SENSOR_DEBUG_H_
#define _BEHAVIOUR_SENSOR_DEBUG_H_

#include "behavior.h"
#include "Controller.h"
#include "Logger.h"
#include "battery.h"
#include "sensor.h"
#include "definitions.h"


class SensorDebug : public BEHAVIOR { 
    public:
        SensorDebug(Controller *controller_, LOGGER *logger_, BATTERY *battery_, SENSOR *leftSensor_, SENSOR *rightSensor_);
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
        unsigned long lastPrint = 0;
};

#endif