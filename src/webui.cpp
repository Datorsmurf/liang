#include "SPIFFS.h"
#include "webui.h"
#include "WebSocketsServer.h"
#include "WebServer.h"
#include "Logger.h"


WEBUI::WEBUI(WebServer* server_, WebSocketsServer* webSocketServer_, LOGGER* logger_, ModeSelectEvent modeSelectEvent_){
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
  Serial.printf("webSocketEvent(%d, %d, ...)\r\n", num, type);
         const char* pl = (const char *)payload;

  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\r\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocketServer->remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        logger->log("Welcome " + String(num), false);
        logger->sendLogHistory(num);
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\r\n", num, payload);

      if (strstr(pl, "mode_") != NULL) {
        if (strstr(pl, "_idle") != NULL) {
          modeSelectEvent(0);
        } else if (strstr(pl, "_mow") != NULL) {
          modeSelectEvent(1);
        } else if (strstr(pl, "_once") != NULL) {
          modeSelectEvent(2);
        } else if (strstr(pl, "_charge") != NULL) {
          modeSelectEvent(3);
        } 
      }

      break;
    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\r\n", num, length);
      //hexdump(payload, length);

      // echo data back to browser
      webSocketServer->sendBIN(num, payload, length);
      break;
    case WStype_ERROR:
      break;

    default:
      Serial.printf("Invalid WStype [%d]\r\n", type);
      break;
  }
}


void WEBUI::handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server->uri();
  message += "\nMethod: ";
  message += (server->method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server->args();
  message += "\n";
  for (uint8_t i=0; i<server->args(); i++){
    message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
  }
  server->send(404, "text/plain", message);
}


 void WEBUI::handleRoot()
{
  Serial.println("handleroot");
  File f = SPIFFS.open("/index.html");
  if (!f) {
    handleNotFound();
    return;
  }

  server->send(200, "text/html",f.readString());
  f.close();
}

void WEBUI::setup() {

File root = SPIFFS.open("/");
File file = root.openNextFile();
  while(file){
 
      Serial.print("FILE: ");
      Serial.println(file.name());
 
      file = root.openNextFile();
}
    server->on("/", std::bind(&WEBUI::handleRoot, this));
    server->onNotFound(std::bind(&WEBUI::handleNotFound, this));
    server->begin();

    webSocketServer->begin();
    webSocketServer->onEvent(std::bind(&WEBUI::webSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
}
void WEBUI::handle() {
    server->handleClient();
}
