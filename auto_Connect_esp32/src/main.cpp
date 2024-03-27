#include <Arduino.h>
#include <WiFi.h>          // Replace with WiFi.h for ESP32
#include <WebServer.h>     // Replace with WebServer.h for ESP32
#include <AutoConnect.h>
#include <Arduino_JSON.h>
#include "index.h"
boolean sendFlag=false;
JSONVar readings;


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

void sendBackValues(){
String json = getSensorReadings();
    Server.send(200, "application/json", json);
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();

  Serial.print("Setting up the Access Point: ");
  Server.on("/htmlContent", rootPage);
 
  // Add a new handler for the button click action
  Server.on("/DATA", sendTextHandler);
  Server.on("/readings",sendBackValues);

  if (Portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }
}

void loop() {
  Portal.handleClient();
}
