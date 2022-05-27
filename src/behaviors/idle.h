#ifndef _BEHAVIOUR_IDLE_H_
#define _BEHAVIOUR_IDLE_H_

#include "behavior.h"
#include "Controller.h"
#include "Logger.h"
#include "battery.h"
#include "sensor.h"
#include "interaction/hardwarebutton.h"

class Idle : public BEHAVIOR { 
    public:
        Idle(Controller *controller_, LOGGER *logger_, BATTERY *battery_, HardwareButton *button_);
        void start();
        int loop();
        int id();
        String desc();
    private:
        Controller *controller;
        LOGGER *logger;
        BATTERY *battery;
        HardwareButton *button;
        unsigned long t = 0;
};

#endif