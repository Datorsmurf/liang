#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "Logger.h"
#include "Controller.h"
#include "definitions.h"
#include "operational_modes/operationalmode.h"
#include "EEPROM.h"

class SETTINGS { 
    public:
        SETTINGS();
        void setup(LOGGER *logger_);

        void commit();

        String readWifiSid();
        void writeWifiSid(String v);

        String readWifiPwd();
        void writeWifiPwd(String v);

        uint16_t readInitialMode();
        void writeInitialMode(uint16_t v);

        uint16_t readWheelMotorLoadLimit();
        void writeWheelMotorLoadLimit(uint16_t v);

        uint16_t readCutterLoadLimit();
        void writeCutterLoadLimit(uint16_t v);

    private:
        LOGGER *logger;

};

#endif