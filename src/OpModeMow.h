#ifndef _OPMODEMOW_H_
#define _OPMODEMOW_H_

#include "operationalmode.h"
#include "Controller.h"
#include "Logger.h"

class OpModeMow : public OPERATIONALMODE { 
    public:
        OpModeMow(Controller *controller_, LOGGER *logger_);
        void start();
        int loop();
        int id();
    private:
        Controller *controller;
        LOGGER *logger;
};

#endif