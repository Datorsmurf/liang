#ifndef _OPMODEIDLE_H_
#define _OPMODEIDLE_H_

#include "operationalmode.h"
#include "Controller.h"
#include "Logger.h"

class OpModeIdle : public OPERATIONALMODE { 
    public:
        OpModeIdle(Controller *controller_, LOGGER *logger_);
        int start();
        int loop();
        int id();
        String desc();
    private:
        Controller *controller;
        LOGGER *logger;
};

#endif