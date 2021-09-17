#ifndef _BEHAVIOUR_LOOK_H_
#define _BEHAVIOUR_LOOK_H_

#include "behavior.h"
#include "Controller.h"
#include "Logger.h"
#include "battery.h"
#include "operational_modes/operationalmode.h"
#include "definitions.h"


class LookForBWF : public BEHAVIOR { 
    public:
        LookForBWF(Controller *controller_, LOGGER *logger_, BATTERY *battery_, ModeSelectEvent modeSelectEvent_, OPERATIONALMODE *currentMode_);
        void start();
        int loop();
        int id();
    private:
        Controller *controller;
        LOGGER *logger;
        BATTERY *battery;
        ModeSelectEvent modeSelectEvent;
        OPERATIONALMODE *currentMode;
};

#endif