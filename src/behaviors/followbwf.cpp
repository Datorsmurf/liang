#include "followbwf.h"
#include "Controller.h"
#include "Logger.h"
#include "definitions.h"



FollowBWF::FollowBWF(Controller *controller_, LOGGER *logger_, BATTERY *battery_) {
    controller = controller_;
    logger = logger_;
    battery = battery_;
    obsticleCountBeforeEvade = 3;
}

void FollowBWF::start() {
    logger->log("Start FollowBwf");
    controller->StopCutter();
    obsticleCount = 0;
    lastObsticle = 0;
}

int FollowBWF::loop() {
    if (controller->IsBumped() || controller->IsTilted()) {
        obsticleCount++;

        if (obsticleCount > obsticleCountBeforeEvade) {
            return BEHAVIOR_GO_AROUND_OBSTICLE;
        }

        controller->StopMovement();
        controller->Move(-30);
    }

    return id();
}

int FollowBWF::id() {
    return BEHAVIOR_FOLLOW_BWF;
}

String FollowBWF::desc() {
    return "Following BWF";
}