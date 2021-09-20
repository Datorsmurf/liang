#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

struct LogEvent {
    unsigned long millis;
    String msg;
};

#define LOG_BUFFER_SIZE 100

class LOGGER { 
    public:
        LOGGER(AsyncWebSocket* webSocketServer_);
        void log(String msg, bool keepInHistory);
        void sendLogHistory(int clientId);

    private:
        

        AsyncWebSocket* webSocketServer;
        LogEvent eventBuffer[LOG_BUFFER_SIZE];
        int bufferPos;
        bool bufferIsLooped = false;
};



#endif