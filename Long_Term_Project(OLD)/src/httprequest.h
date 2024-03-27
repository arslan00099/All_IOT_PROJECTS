#include <Arduino.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include <SPIFFS.h>
unsigned long epouchData;
unsigned long epouchReason;
String rfidSt = "";

int minkelometers,maxkelometers;

const char *reasonrequest = "http://192.168.0.102/pump/getreasonapi.php";
const char *epouch = "http://192.168.0.102/pump/getepoch.php";
const char *serverName = "http://192.168.0.102/pump/getapi.php";

void writeJsonToSpiff(const char *fileName, const char *jsonStr)
{
  File file = SPIFFS.open(fileName, "w"); // open the file for writing
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return;
  }

  file.print(jsonStr); // write the JSON string to the file
  file.close();        // close the file
}

void getData()
{
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);

  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST("{\"rfid\":\"rfidSt\"}");

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0)
  {
    String payload = http.getString();
    Serial.println((String)payload);
    writeJsonToSpiff("/data.json", payload.c_str());
  }

  rfidSt = "";

  // Free resources
  http.end();
}

void getReason()
{
  WiFiClient client;
  HTTPClient http;

  http.begin(client, reasonrequest);

  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST("{\"rfid\":\"rfidSt\"}");

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0)
  {
    String payload = http.getString();
    Serial.println((String)payload);
    writeJsonToSpiff("/reason.json", payload.c_str());
  }

  http.end();
}

void getEpoch()
{
  WiFiClient client;
  HTTPClient http;

  http.begin(client, epouch);

  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST("{\"rfid\":\"rfidSt\"}");

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0)
  {
    String epayload = http.getString();
    Serial.println(epayload);
    DynamicJsonDocument doc(2500);
    DeserializationError error = deserializeJson(doc, epayload);

    // Test if parsing succeeds.
    if (error)
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    Serial.println("SIZE" + (String)doc.size());

    Serial.println("################## EPOUCH ###############");
    String dataep = doc["dataepouch"];
    String reasonep = doc["reasonepouch"];
    minkelometers=doc["minv"];
    maxkelometers=doc["maxv"];
    Serial.println(dataep);
    Serial.println(reasonep);
    unsigned long eData = dataep.toInt();
    unsigned long eReason = reasonep.toInt();

    if (epouchData == eData)
    {
      Serial.println("### SAME EPOCH ###");
    }
    else
    {
      epouchData = eData;
      getData();
    }
    if (epouchReason == eReason)
    {
      Serial.println("### SAME EPOCH ###");
    }
    else
    {
      epouchReason = eReason;
      getReason();
    }
  }

  http.end();
}
