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
    lastPrint = millis();
}

int SensorDebug::loop() {
    if (digitalRead(SWITCH_3_PIN)) {
        return BEHAVIOR_IDLE;
    }

    if (millis() - lastPrint > 500) {
        logger->log(rightSensor->GetPulseHistoryS(), true);
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