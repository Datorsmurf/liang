#ifndef _BEHAVIOUR_LAUNCH_H_
#define _BEHAVIOUR_LAUNCH_H_

#include "behavior.h"
#include "Controller.h"
#include "Logger.h"
#include "battery.h"


class Launch : public BEHAVIOR { 
    public:
        Launch(Controller *controller_, LOGGER *logger_, BATTERY *battery_);
        void start();
        int loop();
        int id();
    private:
        Controller *controller;
        LOGGER *logger;
        BATTERY *battery;
};

#endif