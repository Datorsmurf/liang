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
    logger->log("Start Launch", true);
    controller->StopCutter();
    controller->StopMovement();
}

int Launch::loop() {
    controller->Move(-100);
    controller->Turn(90);

    return BEHAVIOR_MOW;
}

int Launch::id() {
    return BEHAVIOR_LAUNCH;
}