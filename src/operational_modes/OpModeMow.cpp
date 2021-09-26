#include "OpModeMow.h"
#include "Controller.h"
#include "Logger.h"
#include "definitions.h"




OpModeMow::OpModeMow(Controller *controller_, LOGGER *logger_, BATTERY *battery_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
}
int OpModeMow::start() {
    logger->log("Start OpModeMow", true);
    // if (battery->isBeingCharged())  {
    //     if (battery->isFullyCharged()){
    //         return BEHAVIOR_LAUNCH;
    //     } 
    //     return BEHAVIOR_CHARGE;
    // }
    return BEHAVIOR_MOW;
}

int OpModeMow::loop() {
    return id();
}

int OpModeMow::id() {
    return OP_MODE_MOW;
}

String OpModeMow::desc() {
    return "MowRepeat";
}