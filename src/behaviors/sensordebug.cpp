#include "sensordebug.h"



SensorDebug::SensorDebug(Controller *controller_, LOGGER *logger_, BATTERY *battery_, SENSOR *leftSensor_, SENSOR *rightSensor_, HardwareButton *button_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
    leftSensor = leftSensor_;
    rightSensor = rightSensor_;
    button = button_;
}

bool SensorDebug::logSensorChange() {
    return false;
}

void SensorDebug::start() {
    controller->StopCutter();
    controller->StopMovement();
    lastPrint = millis();
    debugLeft = true;
    logger->log("Left sensor");
}

int SensorDebug::loop() {
    if (button->GetConsumablePress()) {
        if (debugLeft) {
            debugLeft = false;
            logger->log("Right sensor");
        }
        else {
            return BEHAVIOR_MOTOR_DEBUG;
        }   
    }


    if (hasTimeout(lastPrint, 2000)) {
        if (debugLeft) {
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