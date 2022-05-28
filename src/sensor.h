#ifndef _SENSOR_H_
#define _SENSOR_H_

#define PULSE_SIGNAL_VALID_MS 3000;
#define PULSE_HISTORY_COUNT 10

#include "Logger.h"
#include "utils.h"

#define LOG_SENSOR_CHANGE false

class SENSOR { 
    public:
        SENSOR(String name_, int pin_, bool missingSignalIsOut_, LOGGER *logger_);
        void setup();
        bool IsIn();
        bool IsOut();
        bool IsOutOfBounds();
        bool IsSignalMissing();
        void IRAM_ATTR handleInterrupt();
        String GetPulseHistoryS();
        

    private:
        static int inside_code[];
        static int outside_code[];

        LOGGER *logger;
        String name;

        int pin;
        bool missingSignalIsOut;
        unsigned long lastSignalTime;
        bool isIn ;
        unsigned long last_pulse;
        int signel_status;
        int pulse_count_inside;
        int pulse_count_outside;
        int pulsehistory[PULSE_HISTORY_COUNT];
        int pulseHistoryPos = 0;
        unsigned long pulseCount = 0;
        bool lastResultWasOutOfBounds = false;
};

#endif