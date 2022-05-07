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
    stepStart = millis();
    currentStep = 0;
    turnAngle = 90;
    turnCount = 0;
}

int MotorDebug::loop() {
    if (digitalRead(SWITCH_3_PIN) == LOW) {
        return BEHAVIOR_IDLE;
    }

    switch (currentStep)
    {
    case 0:
        controller->TurnAngle(turnAngle);
        turnCount = (turnCount+1) % 4;
        if (turnCount == 0) {
            turnAngle = -turnAngle;
        }
        currentStep = 1;
        stepStart = millis();
        break;
    
    case 1:
        controller->StopMovement();;
        if (hasTimeout(stepStart, 500)) {
            currentStep = 2;
            stepStart = millis();
        }
        break;
    case 2:
        controller->RunAsync(FULL_SPEED, FULL_SPEED, NORMAL_ACCELERATION_TIME);
        if (hasTimeout(stepStart, 4000)) {
            controller->StopMovement();;
            currentStep = 0;
            stepStart = millis();

        }
        break;
    
    default:
        break;
    }

    return id();
}

int MotorDebug::id() {
    return BEHAVIOR_MOTOR_DEBUG;
}

String MotorDebug::desc() {
    return "Motor debug";
}