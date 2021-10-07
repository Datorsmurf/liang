#include "charge.h"
#include "Controller.h"
#include "Logger.h"
#include "definitions.h"



Charge::Charge(Controller *controller_, LOGGER *logger_, BATTERY *battery_, OPERATIONALMODE *currentMode_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
    currentMode = currentMode_;
}

void Charge::start() {
    controller->StopMovement();
    controller->StopCutter();
}

int Charge::loop() {
    if (currentMode->id() == OP_MODE_MOW && battery->isFullyCharged()) {
        return BEHAVIOR_LAUNCH;
    }
    return id();
}

int Charge::id() {
    return BEHAVIOR_CHARGE;
}

String Charge::desc() {
    return "Charging";
}