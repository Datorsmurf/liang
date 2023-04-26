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
bool Mow::logSensorChange() {
    return true;
}

void Mow::start() { 
    boostModeSince = 0;
}

int Mow::loop() {

    if (controller->OutOfBoundsTimoutHasOccurred()) return id();
    
    if (battery->mustCharge()) {
        if (mowermodel->CurrentOpModeId == OP_MODE_MOW_ONCE){
            modeSelectEvent(OP_MODE_CHARGE);
            //controller->SetError(ERROR_OUT_OF_BATTERY);
        }
        return BEHAVIOR_LOOK_FOR_BWF;
    }

    if (controller->HandleObsticle(false)) {
        return id();
    }



    if(controller->IsLeftOutOfBounds()) {
        controller->StopMovement();
        t = millis();
        while (!controller->IsFlipped())
        {
            if (!controller->IsLeftOutOfBounds()) {
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
       
        controller->StopMovement();
        t = millis();
        while (!controller->IsFlipped())
        {
            if (!controller->IsRightOutOfBounds()) {
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

    if (boostModeSince == 0 && controller->IsCutterHighLoad() ) {
        boostModeSince = millis();
        logger->log("High cutter load");
    }

    if (boostModeSince > 0) {
        if (hasTimeout(boostModeSince, 10000)) {
            boostModeSince = 0;
        }
        
        controller->RunCutterAsync(CUTTER_SPEED_BOOST);
        controller->RunAsync(LOW_SPEED, LOW_SPEED, NORMAL_ACCELERATION_TIME);

    } else {
        controller->RunCutterAsync(CUTTER_SPEED);
        controller->RunAsync(FULL_SPEED, FULL_SPEED, NORMAL_ACCELERATION_TIME);
    }

    return id();
}

int Mow::id() {
    return BEHAVIOR_MOW;
}

String Mow::desc() {
    return "Mowing";
}