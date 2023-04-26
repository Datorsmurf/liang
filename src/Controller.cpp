#include "Controller.h"
#include "definitions.h"

Controller::Controller(MOTOR* leftMotor_, MOTOR* rightMotor_, MOTOR* cutterMotor_, GYRO* gyro_, BUMPER* bumper_, SENSOR* leftSensor_, SENSOR* rightSensor_, LOGGER* logger_){
    leftMotor = leftMotor_;
    rightMotor = rightMotor_;
    cutterMotor = cutterMotor_;
    gyro = gyro_;
    bumper = bumper_;
    leftSensor = leftSensor_;
    rightSensor = rightSensor_;
    logger = logger_;
}

void Controller::TurnAngle(int degrees){
    degrees = (degrees % 360);
    float targetHeading = Heading() + degrees;
    //logger->log("TurnAngle " + String(degrees) + " start " + String(Heading()) + " target: " + String(targetHeading));
    unsigned long t = millis();

    while (true)
    {
        int angleDiff = abs(targetHeading - Heading());
        if (IsFlipped()) {
            logger->log("Flipped, turn aborted");
            return;
        }
        if (angleDiff < 3) {
            break;
        }


        TurnAsync(degrees < 50 || angleDiff > 35 ? FULL_SPEED : LOW_SPEED, degrees < 0);
        if (hasTimeout(t, 2500)) {
            failedTurnsCount++;
            logger->log("TurnAngle " + String(degrees) + " timed out. Fails in sequence: " + String(failedTurnsCount));
            StopMovement();
            FreezeTargetHeading();
            if (failedTurnsCount > 10) {
                SetError(ERROR_STUCK);
            }
            return;
        }
    }
    failedTurnsCount = 0;
    logger->log("TurnAngle " + String(degrees) + " ok.");
    FreezeTargetHeading();
    StopMovement();
}

void Controller::TurnAsync(int turnSpeed, bool isLeftTurn){
    leftMotor->setSpeed(turnSpeed * (isLeftTurn ? -1 : 1) , SHORT_ACCELERATION_TIME);
    rightMotor->setSpeed(turnSpeed * (isLeftTurn ? 1 : -1), SHORT_ACCELERATION_TIME);
}

bool Controller::RunAsync(int leftSpeed, int rightSpeed, int actionTime){
        int headingDiff = GetTargetHeadingDiff();
        if (headingDiff > 3) {
            leftSpeed = leftSpeed * 0.8;
        } else if (headingDiff < -5) {
            rightSpeed = rightSpeed * 0.8;
        }
    return abs(leftMotor->setSpeed(leftSpeed, actionTime)) + abs(rightMotor->setSpeed(rightSpeed, actionTime)) == 0;
}


void Controller::Move(int distanceInCm){
    logger->log("Move " + String(distanceInCm));
    unsigned long moveEnd = millis() + abs(distanceInCm) * 60;
    if (distanceInCm > 0){
        while (moveEnd > millis())
        {
            leftMotor->setSpeed(FULL_SPEED, NORMAL_ACCELERATION_TIME);
            rightMotor->setSpeed(FULL_SPEED, NORMAL_ACCELERATION_TIME);
            if (IsFlipped()) {
                logger->log("Flipped, aborting move");
                break;
            }
        }
    }

    else {
        while (moveEnd > millis())
        {
            leftMotor->setSpeed(-FULL_SPEED, NORMAL_ACCELERATION_TIME);
            rightMotor->setSpeed(-FULL_SPEED, NORMAL_ACCELERATION_TIME);
            if (IsFlipped()) {
                logger->log("Flipped, aborting move");
                break;
            }
        }
    }   
    StopMovement();
}

void Controller::StopMovement(){
    
    if (leftMotor->getSpeed() == 0 && rightMotor->getSpeed() == 0) {
        //gyro->resetOdometer();
        return;
    }

    while(true)
    {
        rightMotor->setSpeed(0,SHORT_ACCELERATION_TIME);
        leftMotor->setSpeed(0,SHORT_ACCELERATION_TIME);

        if (rightMotor->isAtTargetSpeed() && leftMotor->isAtTargetSpeed()) {
            break;
        }
        delay(1);
    }    
    //delay(3000);
    gyro->resetOdometer();
}

void Controller::RunCutterAsync(int speed){
    cutterMotor->setSpeed(speed, 2000);
}

bool Controller::IsCutterHighLoad() {

    if (!cutterMotor->isOverload(false)) return false;

    if (cutterMotor->getSpeed() < CUTTER_SPEED) return false;

    if (cutterMotor->getLoad() < 10) {
        SetError(ERROR_CUTTER_STUCK);
    }

    return true;
}

void Controller::StopCutter(){
    cutterMotor->setSpeed(0,0);
}

bool Controller::HandleObsticle(bool toughMode){
    if (IsBumped()) {
        logger->log("Bumped");
        DoEvadeObsticle(false);
        return true;
    }

    if (IsTilted()) {
        logger->log("Tilted");
        DoEvadeObsticle(true);
        return true;
    }

    
    if (IsWheelOverload(toughMode)) {
        DoEvadeObsticle(false);
        return true;
    }

    return false;
}

bool Controller::IsBumped() {
    return bumper->IsBumped();
}

bool Controller::IsLeftOutOfBounds() {
    return leftSensor->IsOutOfBounds();
}

bool Controller::IsRightOutOfBounds() {
    return rightSensor->IsOutOfBounds();
}

void Controller::ResetOutOfBoundsTimout(){
    lastTimeInside = millis();
}

void Controller::ResetFailedTurnsCount(){
    failedTurnsCount = 0;
}


bool Controller::OutOfBoundsTimoutHasOccurred(){
    if(!leftSensor->IsOutOfBounds() || rightSensor->IsOutOfBounds()) {
        lastTimeInside = millis();
        return false;
    } else {
        if (hasTimeout(lastTimeInside, 16000)) {
            SetError(ERROR_OUT);
            return true;
        } 
        return false;
    }
}
bool Controller::IsWheelOverload(bool toughMode) {
    return leftMotor->isOverload(toughMode) || rightMotor->isOverload(toughMode);
}

bool Controller::IsTilted() {
    return gyro->getAngleY() > TILT_ANGLE;

}

bool Controller::IsFlipped() {
    return max(abs(gyro->getAngleX()), abs(gyro->getAngleY())) > FLIP_ANGLE;
}

void Controller::ResetNavigation(){
    gyro->setup();
}
int Controller::Heading() {
    return gyro->getHeading();
}

void Controller::FreezeTargetHeading() {
    SetTargetHeading(Heading());
}

void Controller::SetTargetHeading(int heading) {
    overallTargetHeading = heading;
    logger->log("New target heading: " + String(overallTargetHeading));
}

int Controller::GetTargetHeadingDiff() {
    return Heading() - overallTargetHeading;
}


void Controller::SetError(int error_) {
    if (error == error_) return;
    error = error_;
    logger->log("ERROR: " + String(error));
}

int Controller::GetError() {
    return error;
}



void Controller::DoEvadeObsticle(bool smallMovement){
    StopMovement();
    Move(-20);
    int turnAngle = smallMovement ? random(20, 70) : random(90, 160);;
    if (random(0, 100) % 2 == 0) {
        turnAngle = -turnAngle;
    }

    TurnAngle(turnAngle);
}
