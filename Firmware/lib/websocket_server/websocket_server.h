#ifndef WEBSOCKET_SERVER_H
#define WEBSOCKET_SERVER_H

#include <Arduino.h>
//#include <ESPAsyncWebServer.h>

extern AsyncWebSocket ws;
extern AsyncWebSocketClient *client;

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void startWebSocketServer();

#endif
 
