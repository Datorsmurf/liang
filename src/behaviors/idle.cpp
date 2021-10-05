#include "idle.h"
#include "definitions.h"



Idle::Idle(Controller *controller_, LOGGER *logger_, BATTERY *battery_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
}

void Idle::start() {
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
    
    if (hasTimeout(t, 5000)) {
        controller->StopCutter();
    }
     else {
        controller->RunCutterAsync();
    }


    return id();
}

int Idle::id() {
    return BEHAVIOR_IDLE;
}

String Idle::desc() {
    return "Idling";
}