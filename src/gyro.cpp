
#include "gyro.h"

GYRO::GYRO(LOGGER *logger_, MPU6050* mpu_){
    logger = logger_;
    mpu = mpu_;
}


void GYRO::setup() {
    
    byte status = mpu->begin();
    if (status != 0) {
        logger->log("MPU ERROR");
        //ESP.restart();
        initOk = false;
    }

    initOk = true;

  logger->log("Calculating offsets, do not move MPU6050");
  delay(2000);
  mpu->calcOffsets(true,true); // gyro and accelero
  delay(1000);
  //mpu->setFilterGyroCoef(0.98);
}

void GYRO::loop() {

    now = micros();


    if (lastReadTime == 0) {
        mpu->update();
        lastReadTime = now;
        return;
    }

    unsigned long delta =  (now - lastReadTime) ;

    if (delta > 2000) {
        mpu->update();
        lastReadTime = now;
        //Serial.printf("Delta: %lu, Accx: %f\n", delta, mpu->getAccX());
        acceleration = mpu->getAccX() * 10;
        speed = speed + ((acceleration * delta) / 1000000);
        distanceTravelled = distanceTravelled + ((speed * delta) / 1000000);
        filteredY = (filteredY * (1-ANGLE_FILTER)) + (getAngleY() * ANGLE_FILTER);
    }
}

float GYRO::getAcceleration() {
    return acceleration;
}

float GYRO::getSpeed() {
    return speed;
}

float GYRO::getTravelledDistance() {
    return distanceTravelled;
}

void  GYRO::resetOdometer() {
    Serial.println("Reset odometer");
    speed = 0;
    lastReadTime = 0;
    distanceTravelled = 0;
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
