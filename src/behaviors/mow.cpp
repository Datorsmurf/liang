#include "mow.h"
#include "Controller.h"
#include "Logger.h"
#include "definitions.h"



Mow::Mow(Controller *controller_, LOGGER *logger_, BATTERY *battery_, MowerModel* mowerModel_, ModeSelectEvent modeSelectEvent_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
    mowermodel = mowerModel_;
    modeSelectEvent = modeSelectEvent_;
}

void Mow::start() { 
    controller->FreezeTargetHeading();
    
}

int Mow::loop() {

    if (controller->OutOfBoundsTimoutHasOccurred()) return id();
    
    if (battery->mustCharge()) {
        if (mowermodel->CurrentOpModeId == OP_MODE_MOW_ONCE){
            modeSelectEvent(OP_MODE_IDLE);
        }
        return BEHAVIOR_LOOK_FOR_BWF;
    }

    if (controller->HandleObsticle()) {
        return id();
    }



    if(controller->IsLeftOutOfBounds()) {
        logger->log("Left out");
        controller->StopMovement();
        t = millis();
        while (!controller->IsFlipped())
        {
            if (!controller->IsLeftOutOfBounds()) {
                logger->log("Back inside!");
                break;
            }
            if (hasTimeout(t, 5000)) {
                logger->log("Giving up");
                break;
            }
            controller->RunAsync(-FULL_SPEED, -FULL_SPEED, NORMAL_ACCELERATION_TIME);
        }
        controller->StopMovement();
        controller->TurnAngle(120);
        return id();
    }

    if(controller->IsRightOutOfBounds()) {
        logger->log("Right out");
        controller->StopMovement();
        t = millis();
        while (!controller->IsFlipped())
        {
            if (!controller->IsRightOutOfBounds()) {
                logger->log("Back inside!");
                break;
            }
            if (hasTimeout(t, 5000)) {
                logger->log("Giving up");
                break;
            }
            controller->RunAsync(-FULL_SPEED, -FULL_SPEED, NORMAL_ACCELERATION_TIME);
        }
        controller->StopMovement();
        controller->TurnAngle(-120);
        return id();
    }


    controller->RunCutterAsync();
    controller->RunAsync(FULL_SPEED, FULL_SPEED, NORMAL_ACCELERATION_TIME);
    

    return id();
}

int Mow::id() {
    return BEHAVIOR_MOW;
}

String Mow::desc() {
    return "Mowing";
}