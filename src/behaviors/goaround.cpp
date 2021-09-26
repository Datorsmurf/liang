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
    logger->log("Start GoAround", true);
    controller->StopMovement();
    controller->Turn(90);
    startingHeading = controller->Heading();
    startingTime = millis();

}

int GoAround::loop() {
    logger->log("Bailing out and starting to look for BWF instead", true);
    
    //Found BWF
    if (leftSensor->IsOut() || rightSensor->IsOut()) {
        return BEHAVIOR_FOLLOW_BWF;
    }

    //Timeout
    if (startingTime + 10000 < millis()) {
        return BEHAVIOR_LOOK_FOR_BWF;
    }

    if (controller->IsBumped() || controller->IsTilted()) {
        controller->Action_EvadeObsticle();
        return BEHAVIOR_LOOK_FOR_BWF;
    }

    return id();
}

int GoAround::id() {
    return BEHAVIOR_GO_AROUND_OBSTICLE;
}

String GoAround::desc() {
    return "Going around";
}