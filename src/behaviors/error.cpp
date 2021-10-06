#include "error.h"
#include "definitions.h"



Error::Error(Controller *controller_, LOGGER *logger_, BATTERY *battery_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
}

void Error::start() {
    controller->StopCutter();
    controller->StopMovement();
    int checkStart = millis();
    while(checkStart + OPTION_STEP_TIME > millis() && digitalRead(SWITCH_3_PIN) == LOW) {
        delay(1);
    }
}

int Error::loop() {
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

int Error::id() {
    return BEHAVIOR_ERROR;
}

String Error::desc() {
    return "Error";
}