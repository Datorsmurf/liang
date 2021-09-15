#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "Motor.h"

class Controller { 
    public:
        Controller(MOTOR* leftMotor_, MOTOR* rightMotor_, MOTOR* cutterMotor_);
        void Turn(int degrees);
        void RunForward(int speed);
        void StopMovement();
        void RunCutter();
        void StopCutter();

    private:
        MOTOR* leftMotor;
        MOTOR* rightMotor;
        MOTOR* cutterMotor;

};

#endif