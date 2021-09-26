#ifndef _MOWERMODEL_H_
#define _MOWERMODEL_H_
#include "Arduino.h"

struct MowerModel { 
        int CurrentOpModeId;
        float Tilt;
        float Heading;
        int LeftMotorSpeed;
        int LeftMotorLoad;
        int RightMotorSpeed;
        int RightMotorLoad;
        int CutterMotorSpeed;
        int CutterMotorLoad;
        int BatteryVoltage;
        bool LeftSensorIsOutOfBounds;
        bool RightSensorIsOutOfBounds;
        String OpMode;
        String Behavior;
        String message;
};

#endif