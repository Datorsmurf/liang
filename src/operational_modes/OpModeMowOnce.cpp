#include "OpModeMowOnce.h"
#include "Controller.h"
#include "Logger.h"
#include "definitions.h"




OpModeMowOnce::OpModeMowOnce(Controller *controller_, LOGGER *logger_, BATTERY *battery_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
}
int OpModeMowOnce::start() {
    controller->SetError(ERROR_NOERROR);
    if (battery->isBeingCharged())  {
        return BEHAVIOR_LAUNCH;
    }
    return BEHAVIOR_MOW;
}

int OpModeMowOnce::loop() {
    return id();
}

int OpModeMowOnce::id() {
    return OP_MODE_MOW_ONCE;
}

String OpModeMowOnce::desc() {
    return "MowOnce";
}