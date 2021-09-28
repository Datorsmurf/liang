#include <Arduino.h>
#include <Logger.h>
#include <updatehandler.h>
#include "ArduinoOTA.h"
#include "math.h"
#include "operational_modes/operationalmode.h"
UPDATEHANDLER::UPDATEHANDLER(LOGGER *logger_, Controller *controller_, UpdateEvent updateEvent_) {
  logger = logger_;
  controller = controller_;
  updateEvent = updateEvent_;
}

void UPDATEHANDLER::setup() {
  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
   ArduinoOTA.setHostname("liang");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([this]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH){
        type = "sketch";
        controller->StopCutter();
        controller->StopMovement();
        updateEvent(0);
      } 
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      logger->log("Start updating " + type, true);
      lastLogPercent = 0;
     
      updateEvent(0);

    })
    .onEnd([this]() {
      //updateEvent(110);
      logger->log("DONE updating ", true);

    })
    .onProgress([this](unsigned int progress, unsigned int total) {
      int percentDone = floor(((100.0 * progress) / total));
      if (percentDone == 100 || percentDone >= lastLogPercent + 5) {
        //updateEvent(percentDone);
        logger->log("Progress: " + String(percentDone) + "%", false);
        lastLogPercent = percentDone;
      }
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
  
  ArduinoOTA.setPort(8266);
  ArduinoOTA.begin();

  Serial.println("Ready for OTA");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void UPDATEHANDLER::handle() {
  ArduinoOTA.handle();
}

