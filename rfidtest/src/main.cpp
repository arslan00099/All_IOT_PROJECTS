#include <Arduino.h>
#include <WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <SPIFFS.h>
#include "rfidscanner.h"

String ssid ="Islo AI";
String password ="gamican3506";


void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid.c_str(), password.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

SPI.begin();        /*SPI communication initialized*/
    mfrc522.PCD_Init(); /*RFID sensor initialized*/

    if (!SPIFFS.begin(true))
    {
      Serial.println("Failed to mount file system");
      return;
    }


  }


void loop()
{ 
readRFID();
}
