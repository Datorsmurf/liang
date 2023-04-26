#include "charge.h"
#include "Controller.h"
#include "Logger.h"
#include "definitions.h"



Charge::Charge(Controller *controller_, LOGGER *logger_, BATTERY *battery_, MowerModel* mowerModel_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
    mowerModel = mowerModel_;
}
bool Charge::logSensorChange() {
    return false;
}

void Charge::start() {
    controller->StopMovement();
    controller->StopCutter();
    lastCharge = millis();
    wiggleStart = 0;
}

int Charge::loop() {
    if (mowerModel->CurrentOpModeId == OP_MODE_MOW && battery->isFullyCharged()) {
        return BEHAVIOR_LAUNCH;
    }

    if(battery->isBeingCharged()) {
        lastCharge = millis();
        wiggleStart = 0;
    }

    //Give up wiggling after 2 minutes
    if (hasTimeout(lastCharge, 120000)) {
        //logger->log("Giving up wiggling.");
        wiggleStart = 0;
    } else if(hasTimeout(lastCharge, 2000) && wiggleStart == 0) {
        logger->log("Dock connection lost. Trying wiggling to reconnect.");
        wiggleStart = millis();
    }


    //Async wiggling allow for the wiggling to stop immediately when connection is restored.
    if (wiggleStart > 0){
        int i = ((wiggleStart - millis()) / 100) % 200;
        if ( i == 0 || i == 16) {
            controller->RunAsync(0, 190, SHORT_ACCELERATION_TIME);
        } else if (i == 1|| i == 15) {
            controller->RunAsync(190, 0, SHORT_ACCELERATION_TIME);
        }  else {
            controller->StopMovement();
        }
        return id();
    }
    controller->StopMovement();
    return id();
}

int Charge::id() {
    return BEHAVIOR_CHARGE;
}

String Charge::desc() {
    return "Charging";
}