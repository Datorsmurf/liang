#include "motordebug.h"



MotorDebug::MotorDebug(Controller *controller_, LOGGER *logger_, BATTERY *battery_, SENSOR *leftSensor_, SENSOR *rightSensor_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
    leftSensor = leftSensor_;
    rightSensor = rightSensor_;
}

void MotorDebug::start() {
    controller->StopCutter();
    controller->StopMovement();
    int checkStart = millis();
    while(checkStart + OPTION_STEP_TIME > millis() && digitalRead(SWITCH_3_PIN) == LOW) {
        delay(1);
    }   
}

int MotorDebug::loop() {
    if (digitalRead(SWITCH_3_PIN) == LOW) {
        return BEHAVIOR_IDLE;
    }

    unsigned long s = (millis() / 1000) % 8;



    if (s > 5) {
        controller->TurnAngle(90);
        return id();
    }

    if (s > 4) {
        controller->StopMovement();;
        return id();
    }

    controller->RunAsync(FULL_SPEED, FULL_SPEED, NORMAL_ACCELERATION_TIME);

    return id();
}

int MotorDebug::id() {
    return BEHAVIOR_MOTOR_DEBUG;
}

String MotorDebug::desc() {
    return "Motor debug";
}