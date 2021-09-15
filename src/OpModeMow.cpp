#include "OpModeMow.h"
#include "Controller.h"
#include "Logger.h"



OpModeMow::OpModeMow(Controller *controller_, LOGGER *logger_) {
    controller = controller_;
    logger = logger_;
}
void OpModeMow::start() {
    logger->log("Start OpModeMow", true);
}

int OpModeMow::loop() {
    return id();
}

int OpModeMow::id() {
    return 1;
}