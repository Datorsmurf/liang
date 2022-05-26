#include "goaround.h"
#include "Controller.h"
#include "Logger.h"
#include "definitions.h"


GoAround::GoAround(Controller *controller_, LOGGER *logger_, BATTERY *battery_, SENSOR *leftSensor_, SENSOR *rightSensor_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
    leftSensor = leftSensor_;
    rightSensor = rightSensor_;
}

void GoAround::start() {
    logger->log("Start GoAround");
    controller->StopMovement();
    controller->Move(-30);

    controller->TurnAngle(90);
    startingHeading = controller->Heading();
    startingTime = millis();
    newHeadingIsSet = false;

}

int GoAround::loop() {
    if (controller->OutOfBoundsTimoutHasOccurred()) return id();

    //Found BWF
    if (leftSensor->IsOut() || rightSensor->IsOut()) {
        return BEHAVIOR_FOLLOW_BWF;
    }


    if (!newHeadingIsSet && hasTimeout(startingTime, 2000)) {
        controller->SetTargetHeading(startingHeading - 30);
        newHeadingIsSet = true;
    }

    //Reached new heading
    if (newHeadingIsSet && abs(controller->GetTargetHeadingDiff()) <5) {
        logger->log("Reached target heading.");
        return BEHAVIOR_LOOK_FOR_BWF;
    }

    //Timeout or turned reached new heading
    if (hasTimeout(startingTime, 30000)) {
        logger->log("Timeout for going around.");
        return BEHAVIOR_LOOK_FOR_BWF;
    }

    if (controller->HandleObsticle()){
        return BEHAVIOR_LOOK_FOR_BWF;
    }

    controller->RunAsync(FULL_SPEED, FULL_SPEED, NORMAL_ACCELERATION_TIME);

    return id();
}

int GoAround::id() {
    return BEHAVIOR_GO_AROUND_OBSTICLE;
}

String GoAround::desc() {
    return "Going around";
}