#include "Logger.h"

LOGGER::LOGGER(AsyncWebSocket* webSocketServer_){
    webSocketServer = webSocketServer_;
    bufferPos = -1;
    
}

void LOGGER::log(String msg, bool keepInHistory) {
    unsigned long t = millis();
    if (keepInHistory) {
        bufferPos++;
        if (bufferPos >= LOG_BUFFER_SIZE) {
            bufferPos = 0;
            bufferIsLooped = true;
        }

        LogEvent e = eventBuffer[bufferPos];
        e.msg = msg;
        e.millis = t;

        bufferPos = bufferPos % LOG_BUFFER_SIZE;
        eventBuffer[bufferPos] = e;
    }
    
    Serial.println(msg);
    webSocketServer->textAll(String(t) + " " + msg + '\n');
    //webSocketServer->broadcastTXT(String(t) + ": " + msg + '\n');
}



void LOGGER::sendLogHistory(int clientId) {
    webSocketServer->text(clientId, "Catching up with the log...");
    int maxCount = bufferIsLooped ? LOG_BUFFER_SIZE : bufferPos;
    int startAt = bufferIsLooped ? bufferPos : 0;

        for (size_t i = 0; i < maxCount; i++)
        {
            LogEvent e = eventBuffer[(startAt + i) % LOG_BUFFER_SIZE];
            webSocketServer->text(clientId, String(e.millis) + " " + e.msg + '\n');
        }
            webSocketServer->text(clientId, "Done catching up with the log.");

}