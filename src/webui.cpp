#include "SPIFFS.h"
#include "webui.h"
#include "WebSocketsServer.h"
#include "Logger.h"


WEBUI::WEBUI(AsyncWebServer* server_, AsyncWebSocket* webSocketServer_, LOGGER* logger_, ModeSelectEvent modeSelectEvent_){
    webSocketServer = webSocketServer_;
    server = server_;
    logger = logger_;
    modeSelectEvent = modeSelectEvent_;
}

char* subString (const char* input, int offset, int len, char* dest)
{
  int input_len = strlen (input);

  if (offset + len > input_len)
  {
     return NULL;
  }

  strncpy (dest, input + offset, len);
  return dest;
}
void WEBUI::webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  // Serial.printf("webSocketEvent(%d, %d, ...)\r\n", num, type);
  //        const char* pl = (const char *)payload;

  // switch(type) {
  //   case WStype_DISCONNECTED:
  //     Serial.printf("[%u] Disconnected!\r\n", num);
  //     break;
  //   case WStype_CONNECTED:
  //     {
  //       IPAddress ip = webSocketServer->remoteIP(num);
  //       Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
  //       logger->log("Welcome " + String(num), false);
  //       logger->sendLogHistory(num);
  //     }
  //     break;
  //   case WStype_TEXT:
  //     Serial.printf("[%u] get Text: %s\r\n", num, payload);

  //     if (strstr(pl, "mode_") != NULL) {
  //       if (strstr(pl, "_idle") != NULL) {
  //         modeSelectEvent(0);
  //       } else if (strstr(pl, "_mow") != NULL) {
  //         modeSelectEvent(1);
  //       } else if (strstr(pl, "_once") != NULL) {
  //         modeSelectEvent(2);
  //       } else if (strstr(pl, "_charge") != NULL) {
  //         modeSelectEvent(3);
  //       } 
  //     }

  //     break;
  //   case WStype_BIN:
  //     Serial.printf("[%u] get binary length: %u\r\n", num, length);
  //     //hexdump(payload, length);

  //     // echo data back to browser
  //     webSocketServer->sendBIN(num, payload, length);
  //     break;
  //   case WStype_ERROR:
  //     break;

  //   default:
  //     Serial.printf("Invalid WStype [%d]\r\n", type);
  //     break;
  // }
}

void WEBUI::wsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {
if(type == WS_EVT_CONNECT){
    Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
    client->printf("Hello Client %u :)", client->id());
    client->ping();
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

      if(info->opcode == WS_TEXT)
        client->text("I got your text message");
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

void WEBUI::handleNotFound()
{
  // String message = "File Not Found\n\n";
  // message += "URI: ";
  // message += server->uri();
  // message += "\nMethod: ";
  // message += (server->method() == HTTP_GET)?"GET":"POST";
  // message += "\nArguments: ";
  // message += server->args();
  // message += "\n";
  // for (uint8_t i=0; i<server->args(); i++){
  //   message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
  // }
  // server->send(404, "text/plain", message);
}


 void WEBUI::handleRoot()
{
  Serial.println("handleroot");
  File f = SPIFFS.open("/index.html");
  if (!f) {
    handleNotFound();
    return;
  }

  //server->send(200, "text/html",f.readString());
  f.close();
}

void WEBUI::setup() {
  server->addHandler(webSocketServer);
  server->serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    // server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request) {
    //     String message;

    //     if (request->hasParam(PARAM_MESSAGE)) {
    //         message = request->getParam(PARAM_MESSAGE)->value();
    //     } else {
    //         message = "No message sent";
    //     }
    //     request->send(200, "text/html", "Hello, GET: " + message);
    // });
    // server->onNotFound(std::bind(&WEBUI::handleNotFound, this));
    server->begin();
    webSocketServer->onEvent(std::bind(&WEBUI::wsEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
    //webSocketServer->onEvent(std::bind(&WEBUI::webSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
}
void WEBUI::handle() {
//    server->handleClient();
  webSocketServer->cleanupClients();
}
