#include "error.h"
#include "definitions.h"



Error::Error(Controller *controller_, LOGGER *logger_, BATTERY *battery_, MowerModel *mowerModel_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
    mowerModel = mowerModel_;
}

void Error::start() {
    controller->StopCutter();
    controller->StopMovement();
    int checkStart = millis();

    mowerModel->Behavior = getErrorDesc(controller->GetError());
}

String Error::getErrorDesc(int error) {

    switch (error)
    {
    case ERROR_FLIPPED:
        return "Flipped";
    case ERROR_INVALID_BAHAVIOR:
        return "INVALID BEHAVIOR";
    case ERROR_INVALID_OP_MODE:
        return "INVALID MODE";
    case ERROR_OUT:
        return "Out of bounds";
    case ERROR_STUCK:
        return "Mower is stuck";
    case ERROR_BUMPER_STUCK:
        return "Bumper is stuck";
    default:
        return "ERROR " + String(error);
    }

}

int Error::loop() {
    if (digitalRead(SWITCH_3_PIN) == LOW) {
        return BEHAVIOR_SENSOR_DEBUG;
    }

    return id();
}

int Error::id() {
    return BEHAVIOR_ERROR;
}

String Error::desc() {
    return "Error";
}