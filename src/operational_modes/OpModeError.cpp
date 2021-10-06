#include "OpModeError.h"
#include "Controller.h"
#include "Logger.h"



OpModeError::OpModeError(Controller *controller_, LOGGER *logger_) {
    controller = controller_;
    logger = logger_;
}
int OpModeError::start() {
    return BEHAVIOR_ERROR;
}

int OpModeError::loop() {
    return id();
}

int OpModeError::id() {
    return OP_MODE_ERROR;
}


String OpModeError::desc() {
    return "Error";
}