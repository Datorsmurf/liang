#ifndef _OPMODEUPGRADE_H_
#define _OPMODEUPGRADE_H_

#include "operationalmode.h"
#include "Controller.h"
#include "Logger.h"

class OpModeUpgrade : public OPERATIONALMODE { 
    public:
        OpModeUpgrade(Controller *controller_, LOGGER *logger_);
        int start();
        int loop();
        int id();
        String desc();
    private:
        Controller *controller;
        LOGGER *logger;
};

#endif