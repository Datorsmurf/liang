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
        lastObsticle = millis();

        if (obsticleCount > obsticleCountBeforeEvade) {
            return BEHAVIOR_GO_AROUND_OBSTICLE;
        }

        controller->StopMovement();
        controller->Move(-30);
    }

    if (hasTimeout(lastObsticle, 10000)) {
        obsticleCount = 0;
    }

    return id();
}

int FollowBWF::id() {
    return BEHAVIOR_FOLLOW_BWF;
}

String FollowBWF::desc() {
    return "Following BWF";
}