#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "mowermodel.h"
#include "stdio.h"

struct LogEvent {
    unsigned long millis;
    String msg;
};

#define LOG_BUFFER_SIZE 100
#define LOG_MSG_MAX_LENGTH 64

class LOGGER { 
    public:
        LOGGER(AsyncWebSocket* webSocketServer_, MowerModel *model_);
        void log(String msg, bool keepInHistory);
        void sendLogHistory(int clientId);

    private:
        AsyncWebSocket* webSocketServer;
        MowerModel *model;
        
        LogEvent eventBuffer[LOG_BUFFER_SIZE];
        int bufferPos;
        bool bufferIsLooped = false;
};



#endif