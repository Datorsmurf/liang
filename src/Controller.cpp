#include "Controller.h"

Controller::Controller(MOTOR* leftMotor_, MOTOR* rightMotor_, MOTOR* cutterMotor_){
    leftMotor = leftMotor_;
    rightMotor = rightMotor_;
    cutterMotor_ = cutterMotor_;
}

