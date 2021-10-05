#include "SPIFFS.h"
#include "serialui.h"
#include "Logger.h"
#include "operational_modes/operationalmode.h"
#include <vector>

SERIALUI::SERIALUI(ModeSelectEvent modeSelectEvent_){
    modeSelectEvent = modeSelectEvent_;
}


void SERIALUI::PresentMowerModel(MowerModel* model, bool forceFullPresentation) {

}

void SERIALUI::SendLog(LogEvent *e) {
  Serial.println(e->msg);
}
