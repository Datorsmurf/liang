#include "lookforbwf.h"
#include "Controller.h"
#include "Logger.h"
#include "definitions.h"



LookForBWF::LookForBWF(Controller *controller_, LOGGER *logger_, BATTERY *battery_, ModeSelectEvent modeSelectEvent_, OPERATIONALMODE *currentMode_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
    modeSelectEvent = modeSelectEvent_;
    currentMode = currentMode_;
}

void LookForBWF::start() {
    logger->log("Start LookForBwf", true);

    controller->StopCutter();

    if (currentMode->id() == OP_MODE_MOW_ONCE){
        modeSelectEvent(OP_MODE_CHARGE);

    }   
}

int LookForBWF::loop() {
    if (controller->IsBumped()) {
        controller->DoEvadeObsticle();
    }

    if (controller->IsTilted()) {
        controller->DoEvadeObsticle();
    }
    
    controller->Run(100, 100, 300);

    return id();
}

int LookForBWF::id() {
    return BEHAVIOR_LOOK_FOR_BWF;
}

String LookForBWF::desc() {
    return "Looking for BWF";
}