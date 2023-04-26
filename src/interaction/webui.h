#ifndef _WEBUI_H_
#define _WEBUI_H_

#include <ESPAsyncWebServer.h>
#include "EEPROM.h"
#include "Logger.h"
#include "interaction/presenter.h"
#include "operational_modes/operationalmode.h"
#include "logevent.h"
#include "settings.h"

#include "utils.h"
#include "definitions.h"
#include <map>
#include <vector>
#include <ArduinoJson.h>
#include "SPIFFS.h"

class WEBUI : public PRESENTER { 
    public:

        WEBUI(AsyncWebServer* server_, AsyncWebSocket* webSocketServer_, ModeSelectEvent modeSelectEvent_, RebootNeededEvent rebootNeededEvent_, SETTINGS *settings_);
        void setup();
        void doLoop();
        void SetLogger(LOGGER* logger_);
        void PresentMowerModel(MowerModel* model, bool forceFullPresentation);
        void SendLog(LogEvent *e);


    private:
        bool sendToClients(String prefix, String data, unsigned int minSilentTime = 1000);
        bool sendToClients(String prefix, int data, unsigned int minSilentTime = 1000);
        void sendDoc(DynamicJsonDocument doc, uint32_t clientId);
        void wsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
        void handleNotFound(AsyncWebServerRequest *request);
        String processSettings(const String& var);
        //bool enqueueMessage(uint32_t clientId, const String& msg);
        bool hasPrefixSilentTimePassed(String prefix, String data, unsigned int minSilentTime);
        ModeSelectEvent modeSelectEvent;
        AsyncWebSocket* webSocketServer;
        RebootNeededEvent rebootNeededEvent;
        AsyncWebServer* server;
        MowerModel *printedModel;
        SETTINGS *settings;
        std::map<String, unsigned long> prefixSendTimes = std::map<String, unsigned long>();
        bool forceFullPrint = true;
        unsigned long dataThrottleTime = 1000; //Time to wait before sending everchanging data such as voltage or motorloads.
        unsigned long sendTimeForThrottledData = 0;
        std::vector<uint32_t> loggingClients;
        uint32_t clientWaitingForFullLog = 0;

        LOGGER* logger;
};


#endif