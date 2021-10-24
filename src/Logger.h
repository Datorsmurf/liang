#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <Arduino.h>
#include "logevent.h"
#include "interaction/presenter.h"
#include <stdio.h>
#include <vector>
#include "definitions.h"

#define LOG_MSG_MAX_LENGTH 64

class LOGGER { 
    public:
        LOGGER(std::vector<PRESENTER*> &logtargets_);
            
        void log(String msg);
        void getLogHistory(std::vector<LogEvent> *events);

    private:
        //Circular buffer
        LogEvent eventBuffer[LOG_BUFFER_SIZE];
        int bufferPos = -1;
        bool bufferIsLooped = false;


        std::vector<PRESENTER*> logTargets;
};



#endif