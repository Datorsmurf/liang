#include "goaround.h"
#include "Controller.h"
#include "Logger.h"
#include "definitions.h"



GoAround::GoAround(Controller *controller_, LOGGER *logger_, BATTERY *battery_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
}

void GoAround::start() {
    logger->log("Start GoAround", true);
}

int GoAround::loop() {
    controller->StopMovement();
    controller->Turn(90);
    logger->log("Bailing out and starting to look for BWF instead", true);
    return BEHAVIOR_LOOK_FOR_BWF;
}

int GoAround::id() {
    return BEHAVIOR_GO_AROUND_OBSTICLE;
}