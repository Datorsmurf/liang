#include "SPIFFS.h"
#include "webui.h"
#include "Logger.h"
#include "operational_modes/operationalmode.h"
#include <vector>

WEBUI::WEBUI(AsyncWebServer* server_, AsyncWebSocket* webSocketServer_, ModeSelectEvent modeSelectEvent_){
    webSocketServer = webSocketServer_;
    server = server_;
    modeSelectEvent = modeSelectEvent_;
    printedModel = new MowerModel();
}

void WEBUI::SetLogger(LOGGER* logger_) {
  logger = logger_;
}

bool WEBUI::sendToClients(String prefix, int data, unsigned int minSilentTime) {
  if (!WiFi.isConnected()) return false;
  return sendToClients(prefix, String(data), minSilentTime);
}

bool WEBUI::sendToClients(String prefix, String data, unsigned int minSilentTime) {
  if (!WiFi.isConnected()) return false;

  //Serial.printf("%s, %s, %d", prefix.c_str(), data.c_str(), minSilentTime);

  auto search = preixSendTimes.find(prefix);
  if (!webSocketServer->availableForWriteAll()) {
    Serial.println("Websocket not available for write all, skipping message");
    
    return true;
  }
  if ((search == preixSendTimes.end()) || hasTimeout(search->second, minSilentTime)) {
    //Serial.printf("Sendall: %s;%s\n", prefix.c_str(), data.c_str());
    webSocketServer->printfAll("%s;%s", prefix.c_str(), data.c_str());
    
    preixSendTimes[prefix] = millis();
    return true;
  } 
  return false;
}

void WEBUI::PresentMowerModel(MowerModel* model, bool forceFullPresentation) {
  webSocketServer->cleanupClients();

  forceFullPresentation = forceFullPresentation || forceFullPrint;
  forceFullPrint = false;

  if (forceFullPresentation || printedModel->CurrentOpModeId != model->CurrentOpModeId) {
    if(sendToClients("CurrentOpModeId", model->CurrentOpModeId)) {
		  printedModel->CurrentOpModeId = model->CurrentOpModeId;
	  }
  }

  if (forceFullPresentation ||round(printedModel->Tilt) != round(model->Tilt)) {
    if(sendToClients("Tilt", String(model->Tilt, 0))) {
		  printedModel->Tilt = model->Tilt;
	  }
  }

  if (forceFullPresentation ||round(printedModel->Heading) != round(model->Heading)) {
    if(sendToClients("Heading", String(model->Heading, 0))) {
		  printedModel->Heading = model->Heading;
	  }
  }

  if (forceFullPresentation ||printedModel->LeftMotorSpeed != model->LeftMotorSpeed) {
    if(sendToClients("LeftMotorSpeed", model->LeftMotorSpeed, 100)) {
		  printedModel->LeftMotorSpeed = model->LeftMotorSpeed;
	  }
  }

  if (forceFullPresentation ||printedModel->LeftMotorLoad != model->LeftMotorLoad) {
    if(sendToClients("LeftMotorLoad", model->LeftMotorLoad)) {
		  printedModel->LeftMotorLoad = model->LeftMotorLoad;
	  }
  }
  
  if (forceFullPresentation ||printedModel->RightMotorSpeed != model->RightMotorSpeed) {
    if(sendToClients("RightMotorSpeed", model->RightMotorSpeed, 100)) {
		  printedModel->RightMotorSpeed = model->RightMotorSpeed;
	  }
  }
  
  if (forceFullPresentation ||printedModel->RightMotorLoad != model->RightMotorLoad) {
    if(sendToClients("RightMotorLoad", model->RightMotorLoad)) {
		  printedModel->RightMotorLoad = model->RightMotorLoad;
	  }
  }

  if (forceFullPresentation ||printedModel->CutterMotorSpeed != model->CutterMotorSpeed) {
    if(sendToClients("CutterMotorSpeed", model->CutterMotorSpeed, 100)) {
		  printedModel->CutterMotorSpeed = model->CutterMotorSpeed;
	  }
  }
  
  if (forceFullPresentation ||printedModel->CutterMotorLoad != model->CutterMotorLoad) {
    if(sendToClients("CutterMotorLoad", model->CutterMotorLoad)) {
		  printedModel->CutterMotorLoad = model->CutterMotorLoad;
	  }
  }

  if (forceFullPresentation ||round(printedModel->BatteryVoltage * 100) != round(model->BatteryVoltage * 100)) {
    if(sendToClients("BatteryVoltage", String(model->BatteryVoltage, 2))) {
		  printedModel->BatteryVoltage = model->BatteryVoltage;
	  }
  }

  if (forceFullPresentation ||printedModel->IsDocked != model->IsDocked) {
    if(sendToClients("IsDocked", model->IsDocked ? "Y" : "N")) {
		  printedModel->IsDocked = model->IsDocked;
	  }
  }

  if (forceFullPresentation ||printedModel->LeftSensorIsOutOfBounds != model->LeftSensorIsOutOfBounds) {
    if(sendToClients("LeftSensor", model->LeftSensorIsOutOfBounds ? "OUT" : "IN", 100)) {
		  printedModel->LeftSensorIsOutOfBounds = model->LeftSensorIsOutOfBounds;
	  }
  }

  if (forceFullPresentation ||printedModel->RightSensorIsOutOfBounds != model->RightSensorIsOutOfBounds) {
    if(sendToClients("RightSensor", model->RightSensorIsOutOfBounds ? "OUT" : "IN", 100)) {
		  printedModel->RightSensorIsOutOfBounds = model->RightSensorIsOutOfBounds;
	  }
  }

  if (forceFullPresentation ||printedModel->OpMode.compareTo(model->OpMode) != 0) {
    if(sendToClients("OpMode", model->OpMode)) {
		  printedModel->OpMode = model->OpMode;
	  }
  }

  if (forceFullPresentation ||printedModel->Behavior.compareTo(model->Behavior) != 0) {
    if(sendToClients("Behavior", model->Behavior)) {
		  printedModel->Behavior = model->Behavior;
	  }
  }
}

void WEBUI::SendLog(LogEvent *e) {
  webSocketServer->printfAll("l;%lu %s", e->millis, e->msg.c_str());
}

void WEBUI::wsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {
if(type == WS_EVT_CONNECT){
    Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
    client->ping();
    logger->log("Hello client " + String(client->id()));
    //client->printf("Hello Client %u :)", client->id());
    // client->text("l;Catching up with the log.");
    
    // std::vector<LogEvent> logHistory;

    // logger->getLogHistory(&logHistory);

    // for (size_t i = 0; i < logHistory.size(); i++) 
    // {
    //   client->printf("l;%lu %s", logHistory[i].millis, logHistory[i].msg.c_str());
    // }
    
    // client->text("l;Done catching up with the log.");
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
        if (msg.indexOf("mode_") == 0 ) {
          if (msg.indexOf("_idle") > 0) {
            modeSelectEvent(OP_MODE_IDLE);
          } else if (msg.indexOf("_mow") > 0) {
            modeSelectEvent(OP_MODE_MOW);
          } else if (msg.indexOf("_once") > 0) {
            modeSelectEvent(OP_MODE_MOW_ONCE);
          } else if (msg.indexOf("_charge") > 0) {
            modeSelectEvent(OP_MODE_CHARGE);
          } 
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
}
