#include "idle.h"
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
    int checkStart = millis();
    while(checkStart + OPTION_STEP_TIME > millis() && digitalRead(SWITCH_3_PIN) == LOW) {
        delay(1);
    }

}

int Idle::loop() {
    if (digitalRead(SWITCH_3_PIN) == LOW) {
        return BEHAVIOR_SENSOR_DEBUG;
    }

    return id();
}

int Idle::id() {
    return BEHAVIOR_IDLE;
}

String Idle::desc() {
    return "Idling";
}