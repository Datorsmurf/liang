#ifndef _OPMODECHARGE_H_
#define _OPMODECHARGE_H_

#include "operationalmode.h"
#include "Controller.h"
#include "Logger.h"
#include "battery.h"

class OpModeCharge : public OPERATIONALMODE { 
    public:
        OpModeCharge(Controller *controller_, LOGGER *logger_, BATTERY *battery_);
        int start();
        int loop();
        int id();
    private:
        Controller *controller;
        LOGGER *logger;
        BATTERY *battery;
};

#endif