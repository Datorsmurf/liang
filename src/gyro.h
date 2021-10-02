#ifndef _GYRO_H_
#define _GYRO_H_

#include <MPU6050_light.h>

#include "Logger.h"


class GYRO { 
    public:
        GYRO(LOGGER *logger_, MPU6050* mpu_);
        void setup();
        void loop();
        float getHeading();
        float getTilt();
    private:
        LOGGER *logger;
        MPU6050 *mpu;

        float headingInternal = 0;
        unsigned long lastReadTime = 0;

        unsigned long now = micros();

};

#endif