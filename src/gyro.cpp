
#include "gyro.h"

GYRO::GYRO(LOGGER *logger_, MPU6050* mpu_){
    logger = logger_;
    mpu = mpu_;
}


void GYRO::setup() {
    
    byte status = mpu->begin();
    if (status != 0) {
        logger->log("MPU ERROR", true);
        ESP.restart();
    }

  logger->log("Calculating offsets, do not move MPU6050", true);
  mpu->calcOffsets(true,true); // gyro and accelero
  delay(1000);
  //mpu->setFilterGyroCoef(0.98);
}

void GYRO::loop() {

    now = micros();
    mpu->update();
    
    if (lastReadTime != 0) {

        unsigned long delta =  now - lastReadTime;
        //gx/131.0
    }

    
    
    lastReadTime = now;
}

float GYRO::getHeading() {
    return mpu->getAngleZ();
}

float GYRO::getTilt() {
    //This should be som fancy maths, combining the angles :)
    return max(abs(mpu->getAngleX()), abs(mpu->getAngleY()));
}