#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "Motor.h"
#include "gyro.h"
#include "bumper.h"

class Controller { 
    public:
        Controller(MOTOR* leftMotor_, MOTOR* rightMotor_, MOTOR* cutterMotor_, GYRO* gyro_, BUMPER* bumper_);
        void Turn(int degrees);
        void Run(int leftSpeed, int rightSpeed, int actionTime);
        void Move(int distanceInCm);
        void StopMovement();
        void RunCutter(int speed);
        void StopCutter();

        bool IsBumped();
        bool IsTilted();
        int Heading();

        void Action_EvadeObsticle();
    private:
        MOTOR* leftMotor;
        MOTOR* rightMotor;
        MOTOR* cutterMotor;
        GYRO* gyro;
        BUMPER* bumper;

};

#endif