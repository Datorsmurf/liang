#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "Motor.h"
#include "gyro.h"
#include "bumper.h"
#include "sensor.h"

class Controller { 
    public:
        Controller(MOTOR* leftMotor_, MOTOR* rightMotor_, MOTOR* cutterMotor_, GYRO* gyro_, BUMPER* bumper_, SENSOR* leftSensor_, SENSOR* rightSensor_);
        void Turn(int degrees);
        bool Run(int leftSpeed, int rightSpeed, int actionTime);
        void Move(int distanceInCm);
        void StopMovement();
        void RunCutter(int speed);
        void StopCutter();

        int GetError();
        void SetError(int error);

        bool IsBumped();
        bool IsWheelOverload();
        bool IsTilted();
        bool IsFlipped();
        bool IsLeftOutOfBounds();
        bool IsRightOutOfBounds();
        int Heading();

        void DoEvadeObsticle();
    private:
        MOTOR* leftMotor;
        MOTOR* rightMotor;
        MOTOR* cutterMotor;
        GYRO* gyro;
        BUMPER* bumper;
        SENSOR* leftSensor;
        SENSOR* rightSensor;

        int error = 0;

};

#endif