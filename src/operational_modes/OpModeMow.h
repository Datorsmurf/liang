#ifndef _OPMODEMOW_H_
#define _OPMODEMOW_H_

#include "operationalmode.h"
#include "Controller.h"
#include "Logger.h"
#include "battery.h"

class OpModeMow : public OPERATIONALMODE { 
    public:
        OpModeMow(Controller *controller_, LOGGER *logger_, BATTERY *battery_);
        int start();
        int loop();
        int id();
    private:
        Controller *controller;
        LOGGER *logger;
        BATTERY *battery;
};

#endif