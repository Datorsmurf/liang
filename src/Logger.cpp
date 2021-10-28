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

    eventBuffer[bufferPos] = e;
    
    for (size_t i = 0; i < logTargets.size(); i++)
    {
        try
        {
            logTargets[i]->SendLog(&e);
        }
        catch(const std::exception& e)
        {
            Serial.printf("Exception when logging: %s\n", e.what());
        }
    }
}


void LOGGER::getLogHistory(std::vector<LogEvent> *logHistory) {
    int maxCount = bufferIsLooped ? LOG_BUFFER_SIZE : bufferPos+1;
    int startAt = bufferIsLooped ? bufferPos + 1 : 0;
    int pos;

    for (size_t i = 0; i < maxCount; i++)
    {
        pos = (startAt + i) % LOG_BUFFER_SIZE;
        LogEvent e = eventBuffer[pos];
       logHistory->push_back(e);
    }
}