#ifndef _BEHAVIOUR_FOLLOW_H_
#define _BEHAVIOUR_FOLLOW_H_

#include "behavior.h"
#include "Controller.h"
#include "Logger.h"
#include "battery.h"


class FollowBWF : public BEHAVIOR { 
    public:
        FollowBWF(Controller *controller_, LOGGER *logger_, BATTERY *battery_);
        void start();
        int loop();
        int id();
    private:
        Controller *controller;
        LOGGER *logger;
        BATTERY *battery;

        int obsticleCount;
        int obsticleCountBeforeEvade;
        unsigned long lastObsticle;
};

#endif