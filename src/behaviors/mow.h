#ifndef _BEHAVIOUR_MOW_H_
#define _BEHAVIOUR_MOW_H_

#include "behavior.h"
#include "Controller.h"
#include "Logger.h"
#include "battery.h"
#include "utils.h"


class Mow : public BEHAVIOR { 
    public:
        Mow(Controller *controller_, LOGGER *logger_, BATTERY *battery_);
        void start();
        int loop();
        int id();
        String desc();
    private:
        Controller *controller;
        LOGGER *logger;
        BATTERY *battery;

        unsigned long t;
};

#endif