#ifndef _UPDATEHANDLER_H_
#define _UPDATEHANDLER_H_

#include "Logger.h"
#include "Controller.h"
#include "definitions.h"
class UPDATEHANDLER { 
    public:
        UPDATEHANDLER(LOGGER *logger_, Controller *controller_, UpdateEvent updateEvent_);
        void setup();
        void handle();

    private:
        LOGGER *logger;
        Controller *controller;
        
        UpdateEvent updateEvent;

        int lastLogPercent = 0;
};

#endif