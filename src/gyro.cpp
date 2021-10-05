
#include "gyro.h"

GYRO::GYRO(LOGGER *logger_, MPU6050* mpu_){
    logger = logger_;
    mpu = mpu_;
}


void GYRO::setup() {
    
    byte status = mpu->begin();
    if (status != 0) {
        logger->log("MPU ERROR");
        ESP.restart();
    }

  logger->log("Calculating offsets, do not move MPU6050");
  delay(2000);
  mpu->calcOffsets(true,true); // gyro and accelero
  delay(1000);
  //mpu->setFilterGyroCoef(0.98);
}

void GYRO::loop() {

    now = micros();
    mpu->update();
    
    // if (lastReadTime != 0) {

    //     unsigned long delta =  now - lastReadTime;
    // }

    filteredY = (filteredY * (1-ANGLE_FILTER)) + (getAngleY() * ANGLE_FILTER);
    
    
    lastReadTime = now;
}

float GYRO::getHeading() {
    return -mpu->getAngleZ();
}

float GYRO::getAngleX() {
    return mpu->getAngleX();
}

float GYRO::getAngleY() {
    return -mpu->getAngleY();
}

float GYRO::getAngleYFiltered() {
    return filteredY;
}
