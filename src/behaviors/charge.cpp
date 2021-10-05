#include "charge.h"
#include "Controller.h"
#include "Logger.h"
#include "definitions.h"



Charge::Charge(Controller *controller_, LOGGER *logger_, BATTERY *battery_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
}

void Charge::start() {
    logger->log("Start Charge");
}

int Charge::loop() {
    return id();
}

int Charge::id() {
    return BEHAVIOR_CHARGE;
}

String Charge::desc() {
    return "Charging";
}