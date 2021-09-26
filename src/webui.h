#ifndef _WEBUI_H_
#define _WEBUI_H_

#include <ESPAsyncWebServer.h>
#include "Logger.h"
#include "definitions.h"


class WEBUI { 
    public:

        WEBUI(AsyncWebServer* server_, AsyncWebSocket* webSocketServer_, LOGGER* logger_, ModeSelectEvent modeSelectEvent_);
        void setup();
        void handle();

    private:
        void wsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
        void handleNotFound();
        void handleNotFound2(AsyncWebServerRequest *request);
        void handleRoot();
        ModeSelectEvent modeSelectEvent;
        AsyncWebSocket* webSocketServer;
        AsyncWebServer* server;
        LOGGER* logger;
};


#endif