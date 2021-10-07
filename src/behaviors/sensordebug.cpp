#include "sensordebug.h"



SensorDebug::SensorDebug(Controller *controller_, LOGGER *logger_, BATTERY *battery_, SENSOR *leftSensor_, SENSOR *rightSensor_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
    leftSensor = leftSensor_;
    rightSensor = rightSensor_;
}

void SensorDebug::start() {
    controller->StopCutter();
    controller->StopMovement();
    int checkStart = millis();
    while(checkStart + OPTION_STEP_TIME > millis() && digitalRead(SWITCH_3_PIN) == LOW) {
        delay(1);
    }
    lastPrint = millis();
}

int SensorDebug::loop() {
    if (digitalRead(SWITCH_3_PIN) == LOW) {
        return BEHAVIOR_MOTOR_DEBUG;
    }


    if (millis() - lastPrint > 500) {
        if (millis() / 1000 % 4 > 2) {
            logger->log("L: " + leftSensor->GetPulseHistoryS());
        } else {
            logger->log("R: " + rightSensor->GetPulseHistoryS());
        }
        lastPrint = millis();
    }
    return id();
}

int SensorDebug::id() {
    return BEHAVIOR_SENSOR_DEBUG;
}

String SensorDebug::desc() {
    return "Sensor debug";
}