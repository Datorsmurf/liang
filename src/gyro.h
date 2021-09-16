#ifndef _GYRO_H_
#define _GYRO_H_

#include <MPU6050.h>
#include "helper_3dmath.h"

#include "Logger.h"


class GYRO { 
    public:
        GYRO(LOGGER *logger_, int interruptPin_);
        void setup();
        void loop();
        int getHeading();
        int getTilt();
        void dmpDataReady();
    private:
        LOGGER *logger;
        MPU6050 mpu;
        int interruptPin;
        volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

        // MPU control/status vars
        bool dmpReady = false;  // set true if DMP init was successful
        uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
        uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
        uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
        uint16_t fifoCount;     // count of all bytes currently in FIFO
        uint8_t fifoBuffer[64]; // FIFO storage buffer

        // orientation/motion vars
        Quaternion q;           // [w, x, y, z]         quaternion container
        VectorInt16 aa;         // [x, y, z]            accel sensor measurements
        VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
        VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
        VectorFloat gravity;    // [x, y, z]            gravity vector
        float euler[3];         // [psi, theta, phi]    Euler angle container
        float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector



        // ================================================================
        // ===               INTERRUPT DETECTION ROUTINE                ===
        // ================================================================

        // volatile static bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
        // static void dmpDataReady();

};

#endif