#include "OpModeCharge.h"

OpModeCharge::OpModeCharge(Controller *controller_, LOGGER *logger_, BATTERY *battery_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
}
int OpModeCharge::start() {
    logger->log("Start OpModeCharge", true);
    controller->StopCutter();

    if (battery->isBeingCharged()) return BEHAVIOR_CHARGE;

    return BEHAVIOR_LOOK_FOR_BWF;
}

int OpModeCharge::loop() {
        return id();
    }

int OpModeCharge::id() {
    return OP_MODE_CHARGE;
}