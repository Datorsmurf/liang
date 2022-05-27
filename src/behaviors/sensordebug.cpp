#include "sensordebug.h"



SensorDebug::SensorDebug(Controller *controller_, LOGGER *logger_, BATTERY *battery_, SENSOR *leftSensor_, SENSOR *rightSensor_, HardwareButton *button_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
    leftSensor = leftSensor_;
    rightSensor = rightSensor_;
    button = button_;
}

void SensorDebug::start() {
    controller->StopCutter();
    controller->StopMovement();
    lastPrint = millis();
}

int SensorDebug::loop() {
    if (button->GetConsumablePress()) {
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