#include "launch.h"
#include "Controller.h"
#include "Logger.h"
#include "definitions.h"



Launch::Launch(Controller *controller_, LOGGER *logger_, BATTERY *battery_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
}

void Launch::start() {
    logger->log("Start Launch");
    int finalTargetHeading = controller->Heading() + 130;
    controller->StopCutter();
    controller->StopMovement();
    controller->Move(-30);
    controller->TurnAngle(90);
    controller->Move(40);
    controller->SetTargetHeading(finalTargetHeading);

}

int Launch::loop() {

    return BEHAVIOR_MOW;
}

int Launch::id() {
    return BEHAVIOR_LAUNCH;
}

String Launch::desc() {
    return "Launching";
}