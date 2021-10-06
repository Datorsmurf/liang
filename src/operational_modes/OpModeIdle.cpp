#include "OpModeIdle.h"
#include "Controller.h"
#include "Logger.h"



OpModeIdle::OpModeIdle(Controller *controller_, LOGGER *logger_) {
    controller = controller_;
    logger = logger_;
}
int OpModeIdle::start() {
    controller->SetError(ERROR_NOERROR);
    return BEHAVIOR_IDLE;
}

int OpModeIdle::loop() {
    return id();
}

int OpModeIdle::id() {
    return OP_MODE_IDLE;
}


String OpModeIdle::desc() {
    return "Idle";
}