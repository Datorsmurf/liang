#include "launch.h"
#include "Controller.h"
#include "Logger.h"
#include "definitions.h"



Launch::Launch(Controller *controller_, LOGGER *logger_, BATTERY *battery_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
}

void Launch::start() {
    logger->log("Start MOW", true);
}

int Launch::loop() {
    if (battery->mustCharge()) {
        return BEHAVIOR_LOOK_FOR_BWF;
    }

    if (controller->IsBumped()) {
        controller->Action_EvadeObsticle();
    }

    if (controller->IsTilted()) {
        controller->Action_EvadeObsticle();
    }

    
    controller->RunCutter(CUTTER_SPEED);
    controller->Run(100, 100, 300);

    return id();
}

int Launch::id() {
    return BEHAVIOR_LAUNCH;
}