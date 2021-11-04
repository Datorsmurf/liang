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
    controller->TurnAngle(90);
    startingHeading = controller->Heading();
    startingTime = millis();

}

int GoAround::loop() {
    
    //Found BWF
    if (leftSensor->IsOut() || rightSensor->IsOut()) {
        return BEHAVIOR_FOLLOW_BWF;
    }

    //Timeout or turned too far
    if (startingTime + 30000 < millis() || absDiff(startingHeading, controller->Heading()) > 100) {
      return BEHAVIOR_LOOK_FOR_BWF;
    }

    if (controller->HandleObsticle()){
        return BEHAVIOR_LOOK_FOR_BWF;
    }
    controller->RunAsync(FULL_SPEED * 0.8, FULL_SPEED, NORMAL_ACCELERATION_TIME);

    return id();
}

int GoAround::id() {
    return BEHAVIOR_GO_AROUND_OBSTICLE;
}

String GoAround::desc() {
    return "Going around";
}