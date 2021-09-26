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
    controller->Run(-FULL_SPEED, -FULL_SPEED, NORMAL_ACCELERATION_TIME);
    

    return id();
}

int Launch::id() {
    return BEHAVIOR_LAUNCH;
}

String Launch::desc() {
    return "Launching";
}