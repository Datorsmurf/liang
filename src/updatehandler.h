#ifndef _UPDATEHANDLER_H_
#define _UPDATEHANDLER_H_

#include <WebSocketsServer.h>
#include "Logger.h"
class UPDATEHANDLER { 
    public:
        UPDATEHANDLER(LOGGER *logger_);
        void setup();
        void handle();

    private:
        WebSocketsServer* webSocketServer;
        LOGGER *logger;
        int lastLogPercent = 0;

};

#endif