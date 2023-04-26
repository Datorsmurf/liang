#include "lookforbwf.h"
#include "Controller.h"
#include "Logger.h"
#include "definitions.h"



LookForBWF::LookForBWF(Controller *controller_, LOGGER *logger_, BATTERY *battery_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
}

bool LookForBWF::logSensorChange() {
    return true;
}


void LookForBWF::start() {
    controller->StopCutter();
    controller->FreezeTargetHeading();
}

int LookForBWF::loop() {
    if (controller->OutOfBoundsTimoutHasOccurred()) return id();

    if (battery->isBeingCharged()) {
        return BEHAVIOR_CHARGE;
    }
    
    if (controller->IsLeftOutOfBounds() || controller->IsRightOutOfBounds()) {
        return BEHAVIOR_FOLLOW_BWF;
    }
    
    if (controller->HandleObsticle(false)) {
        return id();
    }

    controller->RunAsync(FULL_SPEED, FULL_SPEED, NORMAL_ACCELERATION_TIME);

    return id();
}

int LookForBWF::id() {
    return BEHAVIOR_LOOK_FOR_BWF;
}

String LookForBWF::desc() {
    return "Looking for BWF";
}