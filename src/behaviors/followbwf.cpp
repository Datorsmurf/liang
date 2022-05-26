#include "followbwf.h"
#include "Controller.h"
#include "Logger.h"
#include "definitions.h"



FollowBWF::FollowBWF(Controller *controller_, LOGGER *logger_, BATTERY *battery_, MOTOR *leftMotor_, MOTOR *rightMotor_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
    leftMotor = leftMotor_;
    rightMotor = rightMotor_;
    obsticleCountBeforeEvade = 3;
}

void FollowBWF::start() {
    logger->log("Start FollowBwf");
    controller->StopCutter();
    obsticleCount = 0;
    lastObsticle = 0;
    lastOutside = millis();
}

int FollowBWF::loop() {
    if (controller->OutOfBoundsTimoutHasOccurred()) return id();

    if (hasTimeout(lastObsticle, 10000)) {
        obsticleCount = 0;
    }

    if (battery->isBeingCharged()) {
        return BEHAVIOR_CHARGE;
    }

    
    if (hasTimeout(lastOutside, 10000)) {
        return BEHAVIOR_LOOK_FOR_BWF;
    }

    if (controller->IsBumped() || controller->IsTilted() || controller->IsWheelOverload()) {

        controller->StopMovement();

        //Just stand here for a while to let the charging be detectable.
        delay(1000);
        if (battery->isBeingCharged()) {
            return BEHAVIOR_CHARGE;
        }


        obsticleCount++;
        lastObsticle = millis();

        if (obsticleCount > obsticleCountBeforeEvade) {
            obsticleCount = 0;
            return BEHAVIOR_GO_AROUND_OBSTICLE;
        }

        controller->Move(-30);
    }

    if (controller->IsRightOutOfBounds()) {
        lastOutside = millis();
        controller->StopMovement();

        unsigned long t = millis();
        while (true)
        {
            if (!controller->IsRightOutOfBounds()) {
                logger->log("Back inside!");
                break;
            }
            if (hasTimeout(t, 2000)) {
                logger->log("Giving up");
                break;
            }
            controller->RunAsync(-FULL_SPEED, -FULL_SPEED, NORMAL_ACCELERATION_TIME);
        }
        controller->TurnAngle(DOCKING_TURN_ANGLE_AFTER_BACK_UP);
        return id();
    }

    if (controller->IsLeftOutOfBounds()) {
        lastOutside = millis();
        leftMotor->setSpeed(DOCKING_WHEEL_HIGH_SPEED, DOCKING_TIME_TO_HIGH_SPEED);
        if (rightMotor->getSpeed() < DOCKING_WHEEL_LOW_SPEED)
            rightMotor->setSpeed(DOCKING_WHEEL_HIGH_SPEED, DOCKING_TIME_TO_HIGH_SPEED);
        else 
            rightMotor->setSpeed(DOCKING_WHEEL_LOW_SPEED, DOCKING_TIME_TO_SLOW_SPEED);
    } else {
        rightMotor->setSpeed(DOCKING_WHEEL_HIGH_SPEED, DOCKING_TIME_TO_HIGH_SPEED);
        if (leftMotor->getSpeed() < DOCKING_WHEEL_LOW_SPEED)
            leftMotor->setSpeed(DOCKING_WHEEL_HIGH_SPEED, DOCKING_TIME_TO_HIGH_SPEED);
        else 
            leftMotor->setSpeed(DOCKING_WHEEL_LOW_SPEED, DOCKING_TIME_TO_SLOW_SPEED);

    }

    return id();
}

int FollowBWF::id() {
    return BEHAVIOR_FOLLOW_BWF;
}

String FollowBWF::desc() {
    return "Following BWF";
}