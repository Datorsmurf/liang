#ifndef _WEBUI_H_
#define _WEBUI_H_

#include <WebSocketsServer.h>
#include <webserver.h>
#include "Logger.h"


class WEBUI { 
    public:
    typedef std::function<void(int newMode)> ModeSelectEvent;

        WEBUI(WebServer* server_, WebSocketsServer* webSocketServer_, LOGGER* logger_, ModeSelectEvent modeSelectEvent_);
        void setup();
        void handle();

    private:
        void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
        void handleNotFound();
        void handleRoot();
        ModeSelectEvent modeSelectEvent;
        WebSocketsServer* webSocketServer;
        WebServer* server;
        LOGGER* logger;

};


#endif