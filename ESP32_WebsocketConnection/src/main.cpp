#include <Arduino.h>
#include <WiFi.h>          // Replace with WiFi.h for ESP32
#include <WebServer.h>     // Replace with WebServer.h for ESP32
#include <WebSocketsServer.h>
#include <AutoConnect.h>
#include <Arduino_JSON.h>
#include "index.h"


boolean sendFlag=true;
JSONVar readings;

const char* ssid = "Islo AI";
const char* password = "gamican3506";

String getSensorReadings()
{

if(sendFlag){
  readings["total"] =String ("10");
  readings["up"] =String ("20");
  readings["down"] =String("30");
  readings["left"] = String("40");
  readings["right"] = String("50");
}

else {
 readings["total"] =String ("0");
  readings["up"] =String ("0");
  readings["down"] =String("0");
  readings["left"] = String("0");
  readings["right"] = String("0");
}
  String jsonString = JSON.stringify(readings);
  Serial.println(jsonString);
  return jsonString;
}



void sendTextHandler() {   
  Server.send(200, "/DATA", "Text received by ESP32");
 
if (sendFlag) {
    sendFlag = false;
     delay(100); 
  }
  else {
  sendFlag = true;
     delay(100);
  }
  Serial.println("SEND FLAG STATUS :"+(String)sendFlag);
}

void rootPage(){
  Server.send(200, "text/html", htmlContent);
}


void sendBackValues()
{
    String json = getSensorReadings();
    webSocket.sendTXT(0, json);
    Serial.println(json);
}

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        break;
    case WStype_CONNECTED:
        {
            IPAddress ip = webSocket.remoteIP(num);
            Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        }
        break;
    case WStype_TEXT:
        Serial.printf("[%u] Received text: %s\n", num, payload);
        break;
    }
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  initWiFi();

  Serial.print("Setting up the Access Point: ");
  Serial.print("Setting up the Access Point: ");
    Server.on("/htmlContent", HTTP_GET, rootPage);
    Server.on("/DATA", HTTP_GET, sendTextHandler);
    Server.on("/readings", HTTP_GET, sendBackValues);

    webSocket.begin();
    webSocket.onEvent(onWebSocketEvent);

    if (Portal.begin())
    {
        Serial.println("WiFi connected: " + WiFi.localIP().toString());
    }
}

void loop() {
 Portal.handleClient();
    webSocket.loop();
}
