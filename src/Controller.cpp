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

void Controller::Run(int leftSpeed, int rightSpeed, int actionTime){
    leftMotor->setSpeed(leftSpeed, actionTime);
    rightMotor->setSpeed(rightSpeed, actionTime);
}

void Controller::Move(int distanceInCm){

}

void Controller::StopMovement(){
    leftMotor->setSpeed(0,100);
    rightMotor->setSpeed(0,100);
}

void Controller::RunCutter(int speed){
    cutterMotor->setSpeed(speed, 3000);
}

void Controller::StopCutter(){
    cutterMotor->setSpeed(0,0);
}

bool Controller::IsBumped() {
    return bumper->IsBumped();
}

bool Controller::IsTilted() {
    return gyro->getTilt() > TILT_ANGLE;
}

int Controller::Heading() {
    return gyro->getHeading();
}


void Controller::Action_EvadeObsticle(){
    StopMovement();
    Move(-20);
    int turnAngle = 100;
    if (rand() % 1 == 0) {
        turnAngle = -turnAngle;
    }

    Turn(turnAngle);
}
