#ifndef _BEHAVIOUR_MOW_H_
#define _BEHAVIOUR_MOW_H_

#include "behavior.h"
#include "Controller.h"
#include "Logger.h"
#include "battery.h"
#include "utils.h"
#include "operational_modes/operationalmode.h"


class Mow : public BEHAVIOR { 
    public:
        Mow(Controller *controller_, LOGGER *logger_, BATTERY *battery_, MowerModel* mowerModel_, ModeSelectEvent modeSelectEvent_);
        void start();
        int loop();
        int id();
        String desc();
        bool logSensorChange();
    private:
        Controller *controller;
        LOGGER *logger;
        BATTERY *battery;
        ModeSelectEvent modeSelectEvent;
        MowerModel* mowermodel;
        unsigned long t;
        unsigned long boostModeSince;
};

#endif