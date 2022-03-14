#ifndef _MOWERMODEL_H_
#define _MOWERMODEL_H_
#include "Arduino.h"

struct MowerModel { 
        int CurrentOpModeId;
        float Tilt;
        float Heading;
        float Acceleration;
        float speed;
        float distanceTravelled;
        int LeftMotorSpeed;
        int LeftMotorLoad;
        int RightMotorSpeed;
        int RightMotorLoad;
        int CutterMotorSpeed;
        int CutterMotorLoad;
        float BatteryVoltage;
        bool IsDocked;
        bool LeftSensorIsOutOfBounds;
        bool RightSensorIsOutOfBounds;
        String OpMode;
        String Behavior;
};

#endif