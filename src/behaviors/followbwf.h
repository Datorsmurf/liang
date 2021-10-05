#ifndef _BEHAVIOUR_FOLLOW_H_
#define _BEHAVIOUR_FOLLOW_H_

#include "behavior.h"
#include "Controller.h"
#include "Logger.h"
#include "battery.h"

//Settings for docking behavour. Alter these to adapt to your mower behavour at docking time
#define DOCKING_WHEEL_HIGH_SPEED 255                //Speed for the fast wheel when downing
#define DOCKING_WHEEL_LOW_SPEED 100                 //Speed for the slower wheel when docking
#define DOCKING_TIME_TO_SLOW_SPEED 1000             //Time used for the slower wheel to slow down to its target speed
#define DOCKING_TIME_TO_HIGH_SPEED 10               //Time used for the fast wheel to speed up to its target speed
#define DOCKING_INSIDE_TIMEOUT 10000                //Time to spend continously on the inside of the BWF before restarting state LOOK_FOR_BWF
#define DOCKING_TURN_AFTER_TIMEOUT 30               //Angle to turn inside-timeout has occurred and the mower is about to start looking for BWF.
#define DOCKING_TURN_ANGLE_AFTER_BACK_UP 40         //If backing up when the inner sensor is out of bounds, then this is the turn angle to try to line up with the BWF.


class FollowBWF : public BEHAVIOR { 
    public:
        FollowBWF(Controller *controller_, LOGGER *logger_, BATTERY *battery_, MOTOR *leftMotor_, MOTOR *rightMotor_);
        void start();
        int loop();
        int id();
        String desc();
    private:
        Controller *controller;
        LOGGER *logger;
        BATTERY *battery;
        MOTOR *leftMotor;
        MOTOR *rightMotor;

        int obsticleCount;
        int obsticleCountBeforeEvade;
        unsigned long lastObsticle;
};

#endif