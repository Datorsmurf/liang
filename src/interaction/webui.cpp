#include "webui.h"

WEBUI::WEBUI(AsyncWebServer* server_, AsyncWebSocket* webSocketServer_, ModeSelectEvent modeSelectEvent_){
    webSocketServer = webSocketServer_;
    server = server_;
    modeSelectEvent = modeSelectEvent_;
    printedModel = new MowerModel();
    loggingClients = std::vector<uint32_t>();
}

void WEBUI::SetLogger(LOGGER* logger_) {
  logger = logger_;
}

bool WEBUI::sendToClients(String prefix, int data, unsigned int minSilentTime) {
  if (!WiFi.isConnected()) return false;
  return sendToClients(prefix, String(data), minSilentTime);
}

bool WEBUI::hasPrefixSilentTimePassed(String prefix, String data, unsigned int minSilentTime) {

  auto search = prefixSendTimes.find(prefix);
  if ((search == prefixSendTimes.end()) || hasTimeout(search->second, minSilentTime)) {
    prefixSendTimes[prefix] = millis();
       return true;
  } 
  return false;
}

void WEBUI::PresentMowerModel(MowerModel* model, bool forceFullPresentation) {
  webSocketServer->cleanupClients();

  if (webSocketServer->count() == 0) {
    return;
  }

  DynamicJsonDocument doc(1024);

  forceFullPresentation = forceFullPresentation || forceFullPrint;
  forceFullPrint = false;

  bool sendThrottledData = forceFullPresentation;

  if (forceFullPresentation || hasTimeout(sendTimeForThrottledData, dataThrottleTime)) {
    sendTimeForThrottledData = millis();
    sendThrottledData = true;
  }

  if (forceFullPresentation || printedModel->CurrentOpModeId != model->CurrentOpModeId) {
    doc["CurrentOpModeId"] =  model->CurrentOpModeId;
		printedModel->CurrentOpModeId = model->CurrentOpModeId;
  }

  if (forceFullPresentation || (sendThrottledData && round(printedModel->Tilt) != round(model->Tilt))) {
    doc["Tilt"] = String(model->Tilt, 0);
		printedModel->Tilt = model->Tilt;
  }

  if (forceFullPresentation || (sendThrottledData && round(printedModel->Heading) != round(model->Heading))) {
    doc["Heading"] = String(model->Heading, 0);
		printedModel->Heading = model->Heading;
  }

  if (forceFullPresentation || (sendThrottledData && round(printedModel->Acceleration * 10000) != round(model->Acceleration * 10000))) {
    doc["Acceleration"] = String(model->Acceleration, 4);
		printedModel->Acceleration = model->Acceleration;
  }

 if (forceFullPresentation || (sendThrottledData && round(printedModel->speed * 10000) != round(model->speed * 10000))) {
    doc["Speed"] = String(model->speed, 4);
		printedModel->speed = model->speed;
  }

  if (forceFullPresentation || (sendThrottledData && round(printedModel->distanceTravelled * 10000) != round(model->distanceTravelled * 10000))) {
    doc["Distance"] = String(model->distanceTravelled, 4);
		printedModel->distanceTravelled = model->distanceTravelled;
  }

  if (forceFullPresentation || printedModel->LeftMotorSpeed != model->LeftMotorSpeed) {
    doc["LeftMotorSpeed"] = model->LeftMotorSpeed;
		printedModel->LeftMotorSpeed = model->LeftMotorSpeed;
  }

  if (forceFullPresentation || (sendThrottledData && printedModel->LeftMotorLoad != model->LeftMotorLoad)) {
    doc["LeftMotorLoad"] = model->LeftMotorLoad;
		printedModel->LeftMotorLoad = model->LeftMotorLoad;	  
  }
  
  if (forceFullPresentation || printedModel->RightMotorSpeed != model->RightMotorSpeed) {
    doc["RightMotorSpeed"] = model->RightMotorSpeed;
		printedModel->RightMotorSpeed = model->RightMotorSpeed;	  
  }
  
  if (forceFullPresentation || (sendThrottledData && printedModel->RightMotorLoad != model->RightMotorLoad)) {
    doc["RightMotorLoad"] = model->RightMotorLoad;
		printedModel->RightMotorLoad = model->RightMotorLoad;	  
  }

  if (forceFullPresentation || printedModel->CutterMotorSpeed != model->CutterMotorSpeed) {
    doc["CutterMotorSpeed"] = model->CutterMotorSpeed;
		printedModel->CutterMotorSpeed = model->CutterMotorSpeed;	  
  }
  
  if (forceFullPresentation || (sendThrottledData && printedModel->CutterMotorLoad != model->CutterMotorLoad)) {
    doc["CutterMotorLoad"] = model->CutterMotorLoad;
		printedModel->CutterMotorLoad = model->CutterMotorLoad;	  
  }

  if (forceFullPresentation || (sendThrottledData && round(printedModel->BatteryVoltage * 100) != round(model->BatteryVoltage * 100))) {
    doc["BatteryVoltage"]  = String(model->BatteryVoltage, 2);
		printedModel->BatteryVoltage = model->BatteryVoltage;	  
  }

  if (forceFullPresentation || printedModel->IsDocked != model->IsDocked) {
    doc["IsDocked"] = model->IsDocked ? "Y" : "N";
		printedModel->IsDocked = model->IsDocked;	  
  }

  if (forceFullPresentation || printedModel->LeftSensorIsOutOfBounds != model->LeftSensorIsOutOfBounds) {
    doc["LeftSensor"] = model->LeftSensorIsOutOfBounds ? "OUT" : "IN";
		printedModel->LeftSensorIsOutOfBounds = model->LeftSensorIsOutOfBounds;
  }

  if (forceFullPresentation || printedModel->RightSensorIsOutOfBounds != model->RightSensorIsOutOfBounds) {
    doc["RightSensor"] = model->RightSensorIsOutOfBounds ? "OUT" : "IN";
		printedModel->RightSensorIsOutOfBounds = model->RightSensorIsOutOfBounds;	  
  }

  if (forceFullPresentation || printedModel->OpMode.compareTo(model->OpMode) != 0) {
    doc["OpMode"] = model->OpMode;
		printedModel->OpMode = model->OpMode;	  
  }

  if (forceFullPresentation || printedModel->Behavior.compareTo(model->Behavior) != 0) {
    doc["Behavior"] = model->Behavior;
		printedModel->Behavior = model->Behavior;
  }

  if (doc.size() == 0) return;

  doc["type"] = "model";
  char buffer[1024];
  size_t len = serializeJsonPretty(doc, buffer);
  
  webSocketServer->textAll(buffer, len);
}
void WEBUI::sendDoc(DynamicJsonDocument doc, uint32_t clientId){
  char buffer[1024 + 128];
  size_t len = serializeJson(doc, buffer);

  //Serial.println(buffer);
  if (clientId > 0) {
    webSocketServer->text(clientId, buffer, len);    
  } else {
    webSocketServer->textAll(buffer, len);
  } 
}

void WEBUI::SendLog(LogEvent *e) {

  if (webSocketServer->count() == 0 ) {
    //Serial.println("Empty clientlist");
    loggingClients.clear();
  } 

  if (loggingClients.size() == 0) {
    //Serial.println("No logging clients");
    return;
  }

  DynamicJsonDocument doc(100);
  doc["type"] = "log";
  doc["time"] = e->millis;
  doc["msg"] = e->msg;
  //enqueueMessage(0, "l;" + String(e->millis) + " " + e->msg);
  
   
  for(const auto &logClientId: loggingClients) {
    
    if (webSocketServer->client(logClientId) == nullptr) {
      // Serial.print("Remove client ");
      // Serial.println(logClientId);
      loggingClients.erase(std::remove(loggingClients.begin(), loggingClients.end(), logClientId), loggingClients.end());  
    } else {
      sendDoc(doc, logClientId);
    }
  }
  
  
}

void WEBUI::wsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {

if(type == WS_EVT_CONNECT){
    Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
    logger->log("Hello client " + String(client->id()));
    forceFullPrint = true;
    
  } else if(type == WS_EVT_DISCONNECT){
    Serial.printf("ws[%s][%u] disconnect\n", server->url(), client->id());
  } else if(type == WS_EVT_ERROR){
    Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
  } else if(type == WS_EVT_PONG){
    Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
  } else if(type == WS_EVT_DATA){
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    String msg = "";
    if(info->final && info->index == 0 && info->len == len){
      //the whole message is in a single frame and we got all of it's data
      Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", info->len);

      if(info->opcode == WS_TEXT){
        for(size_t i=0; i < info->len; i++) {
          msg += (char) data[i];
        }
      } else {
        char buff[3];
        for(size_t i=0; i < info->len; i++) {
          sprintf(buff, "%02x ", (uint8_t) data[i]);
          msg += buff ;
        }
      }
      Serial.printf("%s\n",msg.c_str());
      

      if(info->opcode == WS_TEXT) {
        DynamicJsonDocument doc(100);
        deserializeJson(doc, msg);
        if(doc["component"] == "mode" && doc["value"] == "idle") {
          modeSelectEvent(OP_MODE_IDLE);
        } else if(doc["component"] == "mode" && doc["value"] == "mow") {
          modeSelectEvent(OP_MODE_MOW);
        } else if(doc["component"] == "mode" && doc["value"] == "once") {
          modeSelectEvent(OP_MODE_MOW_ONCE);
        } else if(doc["component"] == "mode" && doc["value"] == "charge") {
          modeSelectEvent(OP_MODE_CHARGE);
        } else if(doc["component"] == "log" && doc["value"] == "start") {
          loggingClients.push_back(client->id());
          clientWaitingForFullLog = client->id();
        } else if(doc["component"] == "log" && doc["value"] == "stop") {
          loggingClients.erase(std::remove(loggingClients.begin(), loggingClients.end(), client->id()), loggingClients.end());
        } else if(doc["component"] == "wifi"){
          logger->log("Settings received...");
          EEPROM.writeString(EEPROM_ADR_WIFI_SSID, doc["ssid"].as<String>());
          EEPROM.writeString(EEPROM_ADR_WIFI_PWD, doc["pwd"].as<String>());
          EEPROM.commit();
          logger->log("WifiSettings saved, restarting...");
          delay(500);
          ESP.restart();
        }
      }
      else
        client->binary("I got your binary message");
    } else {
      //message is comprised of multiple frames or the frame is split into multiple packets
      if(info->index == 0){
        if(info->num == 0)
          Serial.printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
        Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
      }

      Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);

      if(info->opcode == WS_TEXT){
        for(size_t i=0; i < len; i++) {
          msg += (char) data[i];
        }
      } else {
        char buff[3];
        for(size_t i=0; i < len; i++) {
          sprintf(buff, "%02x ", (uint8_t) data[i]);
          msg += buff ;
        }
      }
      Serial.printf("%s\n",msg.c_str());

      if((info->index + len) == info->len){
        Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
        if(info->final){
          Serial.printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
          if(info->message_opcode == WS_TEXT)
            client->text("I got your text message");
          else
            client->binary("I got your binary message");
        }
      }
    }
  }
}


void WEBUI::handleNotFound(AsyncWebServerRequest *request) {
  request->send(404);
}

void WEBUI::setup() {
  server->addHandler(webSocketServer);
  server->on("/settings.html", HTTP_POST, [](AsyncWebServerRequest * request){
    int params = request->params();
    String ssid = "";
    String pwd = "";
    for(int i=0;i<params;i++){
      AsyncWebParameter* p = request->getParam(i);
      if(p->isFile()){ //p->isPost() is also true
        Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
      } else if(p->isPost()){
        Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
        if (p->name() == "ssid") {
          ssid = p->value();
        } else if (p->name() == "pwd") {
          pwd = p->value();
        }
      } else {
        Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
    } // for(int i=0;i<params;i++)

    //logger->log("Settings received...");
    EEPROM.writeString(EEPROM_ADR_WIFI_SSID, ssid);
    EEPROM.writeString(EEPROM_ADR_WIFI_PWD, pwd);
    EEPROM.commit();
    //logger->log("WifiSettings saved, restarting...");
    delay(500);
    ESP.restart();

    request -> send(200);
  }); // server.on
  server->serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
 
  server->onNotFound(std::bind(&WEBUI::handleNotFound, this, std::placeholders::_1));
  server->begin();
  webSocketServer->onEvent(std::bind(&WEBUI::wsEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
}
void WEBUI::doLoop() {
  if (clientWaitingForFullLog > 0) {
      int counter = 0;
      std::vector<LogEvent> logHistory;

      logger->getLogHistory(&logHistory);

      //Serial.printf("Logsize: %ul", logHistory.size());
      DynamicJsonDocument doc(4096 + 1024);
      DynamicJsonDocument logsArrayDoc(4096);
      JsonArray logsArray = logsArrayDoc.to<JsonArray>();
      for (auto logEvent : logHistory)
      {
        DynamicJsonDocument logDoc(100);
        logDoc["time"] = logEvent.millis;
        logDoc["msg"] = logEvent.msg;

        logsArray.add(logDoc);

        if (counter++ > 15) {
          counter = 0;

          doc["type"] = "logs";
          doc["logs"] = logsArray;
          sendDoc(doc, clientWaitingForFullLog);

          logsArray.clear();

        }

      }
      doc["type"] = "logs";
      doc["logs"] = logsArray;
      sendDoc(doc, clientWaitingForFullLog);
      clientWaitingForFullLog = 0;
  }
}
