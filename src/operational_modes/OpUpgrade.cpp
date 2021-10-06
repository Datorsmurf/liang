#include "OpModeUpgrade.h"
#include "Controller.h"
#include "Logger.h"



OpModeUpgrade::OpModeUpgrade(Controller *controller_, LOGGER *logger_) {
    controller = controller_;
    logger = logger_;
}
int OpModeUpgrade::start() {
    controller->SetError(ERROR_NOERROR);
    return BEHAVIOR_IDLE;
}

int OpModeUpgrade::loop() {
    return id();
}

int OpModeUpgrade::id() {
    return OP_MODE_UPGRADE;
}


String OpModeUpgrade::desc() {
    return "Upgrade";
}