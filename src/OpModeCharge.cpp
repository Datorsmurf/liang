#include "OpModeCharge.h"

OpModeCharge::OpModeCharge(Controller *controller_, LOGGER *logger_) {
    controller = controller_;
    logger = logger_;
}
void OpModeCharge::start() {
    logger->log("Start OpModeCharge", true);
}

int OpModeCharge::loop() {
        return id();
    }

int OpModeCharge::id() {
    return 3;
}