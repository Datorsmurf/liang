#include "mow.h"
#include "Controller.h"
#include "Logger.h"
#include "definitions.h"



Mow::Mow(Controller *controller_, LOGGER *logger_, BATTERY *battery_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
}

void Mow::start() {
}

int Mow::loop() {
    if (battery->mustCharge()) {
        logger->log("Must charge", true);
        return BEHAVIOR_LOOK_FOR_BWF;
    }

    if (controller->IsBumped()) {
        Serial.println("Bumped");
        controller->Action_EvadeObsticle();
        return id();
    }

    if (controller->IsTilted()) {
        Serial.println("Tilted");
        controller->Action_EvadeObsticle();
        return id();
    }


    controller->RunCutter(CUTTER_SPEED);
    controller->Run(FULL_SPEED, FULL_SPEED, NORMAL_ACCELERATION_TIME);
    

    return id();
}

int Mow::id() {
    return BEHAVIOR_MOW;
}

String Mow::desc() {
    return "Mowing";
}