#ifndef _BEHAVIOUR_GO_AROUND_H_
#define _BEHAVIOUR_GO_AROUND_H_

#include "behavior.h"
#include "Controller.h"
#include "Logger.h"
#include "battery.h"


class GoAround : public BEHAVIOR { 
    public:
        GoAround(Controller *controller_, LOGGER *logger_, BATTERY *battery_);
        void start();
        int loop();
        int id();
    private:
        Controller *controller;
        LOGGER *logger;
        BATTERY *battery;
};

#endif