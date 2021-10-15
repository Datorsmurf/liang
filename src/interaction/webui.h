#ifndef _WEBUI_H_
#define _WEBUI_H_

#include <ESPAsyncWebServer.h>
#include "Logger.h"
#include "interaction/presenter.h"
#include "logevent.h"

#include "utils.h"
#include "definitions.h"
#include <map>


class WEBUI : public PRESENTER { 
    public:

        WEBUI(AsyncWebServer* server_, AsyncWebSocket* webSocketServer_, ModeSelectEvent modeSelectEvent_);
        void setup();
        void doLoop();
        void SetLogger(LOGGER* logger_);
        void PresentMowerModel(MowerModel* model, bool forceFullPresentation);
        void SendLog(LogEvent *e);


    private:
        bool sendToClients(String prefix, String data, unsigned int minSilentTime = 1000);
        bool sendToClients(String prefix, int data, unsigned int minSilentTime = 1000);
        void wsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
        void handleNotFound(AsyncWebServerRequest *request);
        ModeSelectEvent modeSelectEvent;
        AsyncWebSocket* webSocketServer;
        AsyncWebServer* server;
        MowerModel *printedModel;
        std::map<String, unsigned long> preixSendTimes = std::map<String, unsigned long>();
        bool forceFullPrint = true;
        QueueHandle_t  clientsWaitingForLogDump;

        LOGGER* logger;
};


#endif