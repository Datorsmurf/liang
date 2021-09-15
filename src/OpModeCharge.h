#ifndef _OPMODECHARGE_H_
#define _OPMODECHARGE_H_

#include "operationalmode.h"
#include "Controller.h"
#include "Logger.h"

class OpModeCharge : public OPERATIONALMODE { 
    public:
        OpModeCharge(Controller *controller_, LOGGER *logger_);
        void start();
        int loop();
        int id();
    private:
        Controller *controller;
        LOGGER *logger;
};

#endif