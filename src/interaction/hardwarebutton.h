#ifndef _HARDWAREBUTTON_H_
#define _HARDWAREBUTTON_H_

#include "Logger.h"
#include "utils.h"

class HardwareButton  { 
    public:
        HardwareButton(int pin_, LOGGER *logger_);
        bool GetConsumablePress();
        bool IsPressed();
        
        void check();
    private:
        int pin;
        LOGGER *logger;
        unsigned long pressedAt;
        bool pressed = false;
        bool pressConsumed = false;
};

#endif