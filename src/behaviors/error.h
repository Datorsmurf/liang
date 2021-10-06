#ifndef _BEHAVIOUR_ERROR_H_
#define _BEHAVIOUR_ERROR_H_

#include "behavior.h"
#include "Controller.h"
#include "Logger.h"
#include "battery.h"
#include "sensor.h"


class Error : public BEHAVIOR { 
    public:
        Error(Controller *controller_, LOGGER *logger_, BATTERY *battery_);
        void start();
        int loop();
        int id();
        String desc();
    private:
        Controller *controller;
        LOGGER *logger;
        BATTERY *battery;
        unsigned long t = 0;
};

#endif