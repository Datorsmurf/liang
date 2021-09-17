#ifndef _OPMODEMOW_ONCE_H_
#define _OPMODEMOW_ONCE_H_

#include "operationalmode.h"
#include "Controller.h"
#include "Logger.h"
#include "battery.h"

class OpModeMowOnce : public OPERATIONALMODE { 
    public:
        OpModeMowOnce(Controller *controller_, LOGGER *logger_, BATTERY *battery_);
        int start();
        int loop();
        int id();
    private:
        Controller *controller;
        LOGGER *logger;
        BATTERY *battery;
};

#endif