#include "idle.h"
#include "Controller.h"
#include "Logger.h"
#include "definitions.h"



Idle::Idle(Controller *controller_, LOGGER *logger_, BATTERY *battery_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
}

void Idle::start() {
    logger->log("Start Idle", true);
    controller->StopCutter();
    controller->StopMovement();

}

int Idle::loop() {
    return id();
}

int Idle::id() {
    return BEHAVIOR_IDLE;
}