#include "lookforbwf.h"
#include "Controller.h"
#include "Logger.h"
#include "definitions.h"



LookForBWF::LookForBWF(Controller *controller_, LOGGER *logger_, BATTERY *battery_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
}

void LookForBWF::start() {
    logger->log("Start LookForBwf", true);
    controller->StopCutter();
}

int LookForBWF::loop() {
    if (controller->IsBumped()) {
        controller->Action_EvadeObsticle();
    }

    if (controller->IsTilted()) {
        controller->Action_EvadeObsticle();
    }
    
    controller->Run(100, 100, 300);

    return id();
}

int LookForBWF::id() {
    return BEHAVIOR_LOOK_FOR_BWF;
}