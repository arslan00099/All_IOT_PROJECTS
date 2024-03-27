#include <Arduino.h>
#include <WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <SPIFFS.h>
#include "rfidscanner.h"
#include "finger.h"
#include <ArduinoJson.h>
// #include "index.html"
#include <HardwareSerial.h>
#include "post.h"
#include "webdashboard.h"
String STATION_ID = "test";

boolean postFlag = false;
unsigned long readFingerSensor=0;

#define led 2
HardwareSerial HC12Serial(1); // Use Serial2 for HC-12 communication

#define ssidAP "esp32pumpAP"
#define passwordAP "esp32pumpAP"

void spiffData(String id, String rfid);
void resetSPIFFS();

void setupAP()
{
 
  Serial.println("\n[*] Creating AP");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssidAP, passwordAP);
  Serial.print("[+] AP Created with IP Gateway ");
  Serial.println(WiFi.softAPIP());
}


void setupStationID(){
   if (!SPIFFS.begin(true))
    {
      Serial.println("Failed to mount file system");
      return;
    }
       File file = SPIFFS.open("/data.json", "w");
    if (!file) {
      Serial.println("Failed to open file for writing");
      return;
    }

    // Write the string to the file
   // String data = "{\"1\":{\"rfidnum\":\"1234567\"}}";
   String data="{\"staid\":\""+STATION_ID+"\"}";
    file.print(data);

    // Close the file
    file.close();

    Serial.println("Data written to file successfully");
    spiffData(inputId, lastscanid);
}



void spiffData(String id, String rfid)
{
  const char *jsonFilePath = "/data.json";
  File jsonFile = SPIFFS.open(jsonFilePath, "r");
  if (jsonFile)
  {
    size_t fileSize = jsonFile.size();
    std::unique_ptr<char[]> jsonBuffer(new char[fileSize]);
    jsonFile.readBytes(jsonBuffer.get(), fileSize);

    // Parse the JSON data
    DynamicJsonDocument jsonData(1024);
    DeserializationError error = deserializeJson(jsonData, jsonBuffer.get());
    if (error)
    {
      Serial.println("####### Failed to parse JSON file ##############3");
      resetSPIFFS();
      setupStationID();
      Serial.println(error.f_str());
      return;
    }

    // Close the file
    jsonFile.close();

    // Define the RFID number and kilom value
    // const char* rfidnum = "your_rfid_number";
    // int kilom = 100;

    // Check if the RFID number exists in the JSON data
    if (jsonData.containsKey(id))
    {
      // Update the kilom value
      jsonData[id]["rfidnum"] = rfid;
    }
    else
    {
      // Add the RFID number with the kilom value
      jsonData[id]["rfidnum"] = rfid;
    }

    // Save the updated JSON data to SPIFFS
    jsonFile = SPIFFS.open(jsonFilePath, "w");
    if (jsonFile)
    {
      serializeJson(jsonData, jsonFile);
      jsonFile.close();
      Serial.println("JSON data saved successfully");
      delay(3000);
      serverRfid = "";
      postFlag = true;
    }
    else
    {
      Serial.println("Failed to open JSON file for writing");
    }
  }
  else
  {
    Serial.println("Failed to open JSON file");
  }
}

uint8_t getFingerprintEnroll()
{
  int p = -1;
  Serial2.print("Waiting for valid finger to enroll as #");
  Serial2.println(id);
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
    switch (p)
    {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      serverRfid = "Image Taken";
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p)
  {
  case FINGERPRINT_OK:
    Serial.println("Image converted");
    break;
  case FINGERPRINT_IMAGEMESS:
    Serial.println("Image too messy");
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    return p;
  case FINGERPRINT_FEATUREFAIL:
    Serial.println("Could not find fingerprint features");
    return p;
  case FINGERPRINT_INVALIDIMAGE:
    Serial.println("Could not find fingerprint features");
    return p;
  default:
    Serial.println("Unknown error");
    return p;
  }

  Serial.println("Remove finger");
  serverRfid = "Remove finger";
  delay(5000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER)
  {
    p = finger.getImage();
  }
  Serial.print("ID ");
  Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  serverRfid = "Place same Finger";
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
    switch (p)
    {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      serverRfid = "Image Taken";
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p)
  {
  case FINGERPRINT_OK:
    Serial.println("Image converted");
    break;
  case FINGERPRINT_IMAGEMESS:
    Serial.println("Image too messy");
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    return p;
  case FINGERPRINT_FEATUREFAIL:
    Serial.println("Could not find fingerprint features");
    return p;
  case FINGERPRINT_INVALIDIMAGE:
    Serial.println("Could not find fingerprint features");
    return p;
  default:
    Serial.println("Unknown error");
    return p;
  }

  // OK converted!
  Serial.print("Creating model for #");
  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK)
  {
    Serial.println("Prints matched!");
    serverRfid = "Enrolled Sucessfull";

    spiffData(inputId, lastscanid);

    enrolFlag = false;
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR)
  {
    Serial.println("Communication error");
    return p;
  }
  else if (p == FINGERPRINT_ENROLLMISMATCH)
  {
    Serial.println("Fingerprints did not match");
    return p;
  }
  else
  {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID ");
  Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK)
  {
    Serial.println("Stored!");
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR)
  {
    Serial.println("Communication error");
    return p;
  }
  else if (p == FINGERPRINT_BADLOCATION)
  {
    Serial.println("Could not store in that location");
    return p;
  }
  else if (p == FINGERPRINT_FLASHERR)
  {
    Serial.println("Error writing to flash");
    return p;
  }
  else
  {
    Serial.println("Unknown error");
    return p;
  }

  return true;
}

void registerFiger()
{
  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");

  id = (uint8_t)inputId.toInt();

  if (id == 0)
  { // ID #0 not allowed, try again!
    return;
  }
  Serial.print("Enrolling ID #");
  serverRfid = "Enrolling ID # : " + (String)id;
  Serial.println(id);

  while (!getFingerprintEnroll())
    ;
}

void resetSPIFFS()
{
  SPIFFS.end();
  SPIFFS.format();
  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }
  Serial.println("##########  SPIFF SUCESSFULLY RESET #####################");
}

String getRFIDFromID(const char *filePath, int id)
{
  String rfidNum = "";

  // Mount the SPIFFS file system
  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount SPIFFS");
    return rfidNum;
  }

  // Open the file for reading
  File file = SPIFFS.open(filePath, "r");
  if (!file)
  {
    Serial.println("Failed to open file");
    return rfidNum;
  }

  // Read the file into a String
  String fileData = file.readString();

  // Close the file
  file.close();

  // Parse the JSON data
  StaticJsonDocument<256> jsonDocument;
  DeserializationError error = deserializeJson(jsonDocument, fileData);

  if (error)
  {
    Serial.println("Failed to parse JSON");
    return rfidNum;
  }

  // Search for the RFID number based on the ID
  String idString = String(id);
  JsonObject obj = jsonDocument[idString];

  if (obj.containsKey("rfidnum"))
  {
    rfidNum = obj["rfidnum"].as<String>();
  }

  return rfidNum;
}

void setup()
{
  Serial.begin(115200);

  preferences.begin("my-app", false);
  int val = preferences.getInt("rst"); // if 1 thne post data if 0 then open AP mode and 2 forgetting data
  Serial.println("int value for restart : " + (String)val);

  if ((val == 1))
  {
    Serial.println("###########  POSTING DATA  ########");
    // Post JSON data to the API
    const char *filePath = "/data.json";
    const char *apiURL = "http://espstack.com/pump/fingerprintSideApi.php"; // Replace with your API endpoint URL
    postDataToAPI(filePath, apiURL);
  }
  else if ((val == 2))
  {
    Serial.println("###########  GETTING DATA  ########");
    getDataFromServer();
  }
  else
  {
    Serial.println("###########  RUNING IN NORMAL MODE ########");
     setupAP();
  setupServer();

    HC12Serial.begin(9600, SERIAL_8N1, 35, 22); // Initialize HC-12 module communication (RX: GPIO16, TX: GPIO17)
    pinMode(led, OUTPUT);
    //  setupNExtion();
   //  resetSPIFFS();
    // pinMode(pump_RELAY, OUTPUT);
    // digitalWrite(pump_RELAY, LOW);

    SPI.begin();        /*SPI communication initialized*/
    mfrc522.PCD_Init(); /*RFID sensor initialized*/

    if (!SPIFFS.begin(true))
    {
      Serial.println("Failed to mount file system");
      return;
    }





    File file = SPIFFS.open("/data.json", "r");
    if (!file)
    {
      Serial.println("Failed to open file for reading");
      return;
    }

    // Read and print the contents of the file

    while (file.available())
    {
      Serial.write(file.read());
    }

    setupFinger();
  }

 
   
}

void loop()
{ 

  if (postFlag == true)
  {
    preferences.begin("my-app", false);
    preferences.putInt("rst", 1); // if 1 thne post data if 0 then open AP mode
    preferences.end();
    Serial.println("Swiching from 0 to 1");
    delay(5000);
    ESP.restart();
  }

  if (rfidSt.length() > 5)
  {
    Serial.println("SENDING ");

    HC12Serial.println(rfidSt);
    rfidSt = "";
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
  }

  if (enrolFlag == true)
  {
    Serial.println("#############  RGISTER RFID ##########");
    registerFiger();
  }
  else
  {
    if(millis() - readFingerSensor > 100){
getFingerprintID();


    if (foundID != 0)
    {
      Serial.println("HERE WE GET AN ID : " + (String)foundID);
      const char *filePath = "/data.json";
      int id = foundID; // Enter the ID here

      String rfidNumber = getRFIDFromID(filePath, id);

      if (rfidNumber != "")
      {
        Serial.print("RFID number found: ");
        Serial.println(rfidNumber);
        rfidNumber.trim();
        HC12Serial.println(rfidNumber);
        digitalWrite(led, HIGH);
        delay(1000);
        digitalWrite(led, LOW);
      }
      else
      {
        Serial.println("RFID number not found for the given ID");
      }
      foundID = 0;
    }
    readFingerSensor=millis();
  }
  
readRFID();

}
}