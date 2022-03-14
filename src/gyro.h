#ifndef _GYRO_H_
#define _GYRO_H_

#include <MPU6050_light.h>

#include "Logger.h"
#define ANGLE_FILTER 0.2

class GYRO { 
    public:
        GYRO(LOGGER *logger_, MPU6050* mpu_);
        void setup();
        void loop();
        float getHeading();
        float getAngleX();
        float getAngleY();
        float getAngleYFiltered();
        float getAcceleration();
        float getSpeed();
        float getTravelledDistance();
        void resetOdometer();

    private:
        LOGGER *logger;
        MPU6050 *mpu;

        float headingInternal = 0;
        float acceleration = 0;
        float speed = 0;
        float distanceTravelled = 0;

        unsigned long lastReadTime = 0;

        unsigned long now = micros();
        float filteredY = 0;
        bool initOk = false;
};

#endif