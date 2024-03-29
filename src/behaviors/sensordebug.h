#ifndef _BEHAVIOUR_SENSOR_DEBUG_H_
#define _BEHAVIOUR_SENSOR_DEBUG_H_

#include "behavior.h"
#include "Controller.h"
#include "Logger.h"
#include "battery.h"
#include "sensor.h"
#include "definitions.h"
#include "interaction/hardwarebutton.h"


class SensorDebug : public BEHAVIOR { 
    public:
        SensorDebug(Controller *controller_, LOGGER *logger_, BATTERY *battery_, SENSOR *leftSensor_, SENSOR *rightSensor_, HardwareButton *button_);
        void start();
        int loop();
        int id();
        String desc();
        bool logSensorChange();
    private:
        Controller *controller;
        LOGGER *logger;
        BATTERY *battery;
        SENSOR *leftSensor;
        SENSOR *rightSensor;
        HardwareButton *button;
        unsigned long lastPrint = 0;
        bool debugLeft;
};

#endif