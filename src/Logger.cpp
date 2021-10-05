#include "Logger.h"

LOGGER::LOGGER(std::vector<PRESENTER*> &logtargets_){
    logTargets = logtargets_;
    bufferPos = -1;
}


void LOGGER::log(String msg) {
    
    unsigned long t = millis();
    bufferPos++;
    if (bufferPos >= LOG_BUFFER_SIZE) {
        bufferPos = 0;
        bufferIsLooped = true;
    }

    LogEvent e = eventBuffer[bufferPos];
    e.msg = msg;
    e.millis = t;

    bufferPos = bufferPos % LOG_BUFFER_SIZE;
    eventBuffer[bufferPos] = e;
    
    for (size_t i = 0; i < logTargets.size(); i++)
    {
        try
        {
            logTargets[i]->SendLog(&e);
        }
        catch(const std::exception& e)
        {
            Serial.printf("Exception when logging: %s", e.what());
        }
    }
}


void LOGGER::getLogHistory(std::vector<LogEvent> *logHistory) {
    int maxCount = bufferIsLooped ? LOG_BUFFER_SIZE : bufferPos;
    int startAt = bufferIsLooped ? bufferPos : 0;

    for (size_t i = 0; i < maxCount; i++)
    {
        LogEvent e = eventBuffer[(startAt + i) % LOG_BUFFER_SIZE];
        logHistory->push_back(e);
    }
}