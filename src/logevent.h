

#ifndef _LOGEVENT_H_
#define _LOGEVENT_H_

#include "Arduino.h"

struct LogEvent {
    unsigned long millis;
    String msg;
};

#endif