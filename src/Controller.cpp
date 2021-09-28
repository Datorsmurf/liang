#include "Controller.h"
#include "definitions.h"

Controller::Controller(MOTOR* leftMotor_, MOTOR* rightMotor_, MOTOR* cutterMotor_, GYRO* gyro_, BUMPER* bumper_){
    leftMotor = leftMotor_;
    rightMotor = rightMotor_;
    cutterMotor = cutterMotor_;
    gyro = gyro_;
    bumper = bumper_;
}

void Controller::Turn(int degrees){

}

bool Controller::Run(int leftSpeed, int rightSpeed, int actionTime){
    return leftMotor->setSpeed(leftSpeed, actionTime) + rightMotor->setSpeed(rightSpeed, actionTime) == 0;
}

void Controller::Move(int distanceInCm){
    unsigned long moveEnd = millis() + abs(distanceInCm) / 10;
    if (distanceInCm > 0){
        while (moveEnd > millis())
        {
            leftMotor->setSpeed(FULL_SPEED, NORMAL_ACCELERATION_TIME);
            rightMotor->setSpeed(FULL_SPEED, NORMAL_ACCELERATION_TIME);
            
        }
    }

    else {
        while (moveEnd > millis())
        {
            leftMotor->setSpeed(-FULL_SPEED, NORMAL_ACCELERATION_TIME);
            rightMotor->setSpeed(-FULL_SPEED, NORMAL_ACCELERATION_TIME);
        }

    }   
    StopMovement();
}

void Controller::StopMovement(){
    while(rightMotor->setSpeed(0,SHORT_ACCELERATION_TIME) + leftMotor->setSpeed(0,SHORT_ACCELERATION_TIME) > 0)
    {
        delay(1);
    }
}

void Controller::RunCutter(int speed){
    cutterMotor->setSpeed(speed, 3000);
}

void Controller::StopCutter(){
    cutterMotor->setSpeed(0,0);
}

bool Controller::IsBumped() {
    return false;
    return bumper->IsBumped();
}

bool Controller::IsWheelOverload() {
    
    return leftMotor->isOverload() || rightMotor->isOverload();
}

bool Controller::IsTilted() {
    return gyro->getTilt() > TILT_ANGLE;
}

bool Controller::IsFlipped() {
    return gyro->getTilt() > FLIP_ANGLE;
}


int Controller::Heading() {
    return gyro->getHeading();
}

void Controller::SetError(int error_) {
    error = error_;
}

int Controller::GetError() {
    return error;
}



void Controller::DoEvadeObsticle(){
    StopMovement();
    Move(-20);
    int turnAngle = 100;
    if (rand() % 1 == 0) {
        turnAngle = -turnAngle;
    }

    Turn(turnAngle);
}
