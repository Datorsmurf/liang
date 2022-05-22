#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "Motor.h"
#include "gyro.h"
#include "bumper.h"
#include "sensor.h"
#include "utils.h"
#include "Logger.h"

class Controller { 
    public:
        Controller(MOTOR* leftMotor_, MOTOR* rightMotor_, MOTOR* cutterMotor_, GYRO* gyro_, BUMPER* bumper_, SENSOR* leftSensor_, SENSOR* rightSensor_, LOGGER* logger_);
        void TurnAngle(int degrees);
        void TurnAsync(int turnSpeed, bool isLeftTurn);
        bool RunAsync(int leftSpeed, int rightSpeed, int actionTime);
        void Move(int distanceInCm);
        void StopMovement();
        void RunCutterAsync(int speed);
        bool IsCutterHighLoad();
        void StopCutter();
        bool HandleObsticle();

        int GetError();
        void SetError(int error);

        bool IsBumped();
        bool IsWheelOverload();
        bool IsTilted();
        bool IsFlipped();
        bool IsLeftOutOfBounds();
        bool IsRightOutOfBounds();
        int Heading();
        void FreezeTargetHeading();
        int GetTargetHeadingDiff();
        bool OutOfBoundsTimoutHasOccurred();
        void ResetOutOfBoundsTimout();

        void DoEvadeObsticle();
    private:
        MOTOR* leftMotor;
        MOTOR* rightMotor;
        MOTOR* cutterMotor;
        GYRO* gyro;
        BUMPER* bumper;
        SENSOR* leftSensor;
        SENSOR* rightSensor;
        LOGGER* logger;

        int error = 0;
        int overallTargetHeading = 0;
        unsigned long lastTimeInside = 0;

        int loggedSpeed = 1;
};

#endif