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
    //Serial.println("No client. Skipping presentation");
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

  if (forceFullPresentation || (sendThrottledData && forceFullPresentation ||round(printedModel->Tilt) != round(model->Tilt))) {
    doc["Tilt"] = String(model->Tilt, 0);
		printedModel->Tilt = model->Tilt;
  }

  if (forceFullPresentation || (sendThrottledData && round(printedModel->Heading) != round(model->Heading))) {
    doc["Heading"] = String(model->Heading, 0);
		printedModel->Heading = model->Heading;
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

void WEBUI::SendLog(LogEvent *e) {

  if (webSocketServer->count() == 0 ) {
    Serial.println("Empty clientlist");
    loggingClients.clear();
  } 

  if (loggingClients.size() == 0) {
    Serial.println("No logging clients");
    return;
  }

  DynamicJsonDocument doc(100);
  doc["type"] = "log";
  doc["time"] = e->millis;
  doc["msg"] = e->msg;
  //enqueueMessage(0, "l;" + String(e->millis) + " " + e->msg);
  
  char buffer[1024];
  size_t len = serializeJsonPretty(doc, buffer);
   
  for(const auto &logClientId: loggingClients) {
    
    if (webSocketServer->client(logClientId) == nullptr) {
      Serial.print("Remove client ");
      Serial.println(logClientId);
      loggingClients.erase(std::remove(loggingClients.begin(), loggingClients.end(), logClientId), loggingClients.end());  
    }
    webSocketServer->text(logClientId, buffer, len);
  }
  
  
}

void WEBUI::wsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {
if(type == WS_EVT_CONNECT){
    Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
    //client->ping();
    uint32_t clientId = client->id();
    logger->log("Hello client " + String(clientId));


    // std::vector<LogEvent> logHistory;

    // logger->getLogHistory(&logHistory);
    // Serial.println("Connect 2. Size: " + String(logHistory.size()));
    // for (size_t i = 0; i < logHistory.size()-1; i++) 
    // {
    //   Serial.println("Connect i: " + String(i));

    //   PendingMessage pm;
    //   pm.clientId = clientId;
    //   String msg = "l;" + String(logHistory[i].millis) + " " + logHistory[i].msg;
    //   pm.msg = &msg;
    //   Serial.println("Connect 3");
    //   xQueueSend(pendingMessages, &pm, 0);
    //   Serial.println("Connect 4");
      
    // }

    //xQueueSend(pendingMessages, &clientId, portMAX_DELAY);
    
    //client->printf("Hello Client %u :)", client->id());
    
    forceFullPrint = true;
    
    //logger->sendLogHistory(client->id());
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
          Serial.print("Start log. ");
          Serial.println(loggingClients.size());
        } else if(doc["component"] == "log" && doc["value"] == "stop") {
          Serial.println("Stop log");
          loggingClients.erase(std::remove(loggingClients.begin(), loggingClients.end(), client->id()), loggingClients.end());
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
  server->serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  server->onNotFound(std::bind(&WEBUI::handleNotFound, this, std::placeholders::_1));
  server->begin();
  webSocketServer->onEvent(std::bind(&WEBUI::wsEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
}
void WEBUI::doLoop() {

//webSocketServer->cleanupClients();



  // PendingMessage pendingPeek;
  // Serial.println("0");
  // if (xQueuePeek(pendingMessages, &pendingPeek, 0)) {
  //   Serial.println("10");
  //   if (pendingPeek.clientId == 0) {
  //     Serial.println("15");
  //     PendingMessage pending;
  //     if (webSocketServer->availableForWriteAll() && xQueueReceive(pendingMessages, &pending, 0))
  //     {
  //           Serial.println("16");
  //           Serial.println(String(pending.clientId));
  //             Serial.println("17");
  //             if (pending.msg == nullptr) {
  //               Serial.println("Null msg pointer");

  //             } else {
  //               Serial.println("Not null msg pointer");
  //               Serial.printf("%p", pending.msg);
  //               Serial.println("Apa");
  //             }

  //         String s = *pending.msg;
  //         Serial.println(s);
  //         webSocketServer->textAll(s);
  //             Serial.println("19");
  //     }

  //   } else {
  //     PendingMessage pending;
  //         Serial.println("20");

  //     if (webSocketServer->availableForWrite(pendingPeek.clientId) && xQueueReceive(pendingMessages, &pending, 0))
  //     {
  //         webSocketServer->text(pending.clientId, *pending.msg);
  //     }

  //   }

  //     //webSocketServer->text(clientId, "l;Done catching up with the log.");

  
  
}
