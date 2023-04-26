#include "idle.h"
#include "definitions.h"



Idle::Idle(Controller *controller_, LOGGER *logger_, BATTERY *battery_, HardwareButton *button_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
    button = button_;
}
bool Idle::logSensorChange() {
    return true;
}

void Idle::start() {
    controller->StopCutter();
    controller->StopMovement();
}

int Idle::loop() {
    if (button->GetConsumablePress()) {
        return BEHAVIOR_SENSOR_DEBUG;
    }
    
    controller->StopCutter();
    controller->StopMovement();

    return id();
}

int Idle::id() {
    return BEHAVIOR_IDLE;
}

String Idle::desc() {
    return "Idling";
}