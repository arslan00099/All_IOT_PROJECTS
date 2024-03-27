
#include "key.h"
#include "rfidscanner.h"

#define pump_RELAY 4

// int pos = 0; // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
int servo1Pin = 4;

const char *ssid = "RIGJAZZ";
const char *password = "Kashmiri786";

void setupWifi()
{
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void printlnMillis()
{
  if (millis() - preMillis > 1000)
  {
    // Serial.println("FLAG MILLIS :"+(String)flagcheckMillis);
    //    Serial.println("MILLIS :"+(String)preMillis);
    preMillis = millis();
  }
}

void printinfo()
{
  for (int i = 0; i < 3; i++)
  {
    Serial.println(userArray[i]);
    Serial.println(vechileArray[i]);
  }

  for (int i = 0; i < 2; i++)
  {
    Serial.println(info[i]);
  }
}

void resetSPIFFS()
{
  // Unmount the SPIFFS file system
  SPIFFS.end();

  // Format the SPIFFS file system
  SPIFFS.format();

  // Mount the SPIFFS file system
  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }
}

void setup()
{
  Serial.begin(115200);
  // resetSPIFFS();
  pinMode(pump_RELAY, OUTPUT);
  digitalWrite(pump_RELAY, LOW);

  SPI.begin();
  Wire.begin();
  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }

  setupWifi();

  mfrc522.PCD_Init();
  Serial.println("Approach your reader card...");
  Serial.println();
  
}

void loop()
{
  printlnMillis();
  readRFID();
  loopkeypad();
  if (millis() - checkepoch > 15000)
  {
    getEpoch();
    checkepoch = millis();
  }

  if ((userFlag == true) || (vechileFlag == true))
  {
    if ((millis() - flagcheckMillis < 15000) && (userFlag == true) && (vechileFlag == true))
    {
      Serial.println("BOTH ARE ASSIGNED");
    //  main_MENU(0);
    Serial.println("MAIN MENU WILL BE OPENED");
      reasonMenu = 1;
      userFlag = false;
      vechileFlag = false;
    }
    else
    {
      if (millis() - flagcheckMillis > 15000)
      {
        Serial.println("FLAG RESET");
        Serial.println ("SCAN AGAIN");
        userFlag = false;
        vechileFlag = false;
      }
    }
  }
  else
  {
    //  Serial.println("ENTER SECOND TAG");
  }

}