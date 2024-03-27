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
//#include "hc12.h"
String station_ID = "1";
String rfidSt = "";

int minkelometers, maxkelometers;
float calibrationFactor;
const int MAX_RECORDS = 500;
String vechileRfidArray[MAX_RECORDS];
String keloArray[MAX_RECORDS];

const char *reasonrequest = "http://192.168.0.103/pump/getreasonapi.php";
const char *epouch = "http://192.168.0.103/pump/getepoch.php";
const char *serverName = "http://192.168.0.103/pump/getapi.php";
const char *postdata = "http://192.168.0.103/pump/postApi.php";

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
  int httpResponseCode = http.POST("{\"staid\":\"" + station_ID + "\"}");

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

    Serial.println("################## EPOUCH ###############");
    String dataep = doc["dataepouch"];
    String reasonep = doc["reasonepouch"];
    int minimunV = doc["minv"];
    int maximumV = doc["maxv"];
    float CF = doc["value"];

    for (int i = 0; i < doc.size() - 5; i++)
    {
      String vechilerfid = doc[String(i)]["vechilerfid"];
      String kilom = doc[String(i)]["kilom"];
      vechileRfidArray[i] = vechilerfid;
      keloArray[i] = kilom;
      Serial.println("VEchile M1 : " + (String)vechileRfidArray[i]);
      Serial.println("KELO M2 : " + (String)keloArray[i]);
    }

    // #################   SAVING INTO SPIFF ###########################333
    if ((minimunV != minkelometers) || (maximumV != maxkelometers) || (CF != calibrationFactor))
    {
      Serial.println("###################   SAVED MIN MAX SPIFF ##################");
      String payload = "{\"minK\":\"" + (String)minimunV + "\",\"maxK\":\"" + (String)maximumV + "\",\"cf\":\"" + (String)CF + "\"}";
      writeJsonToSpiff("/minMaxKelo.json", payload.c_str());
      minkelometers = minimunV;
      maxkelometers = maximumV;
      calibrationFactor = CF;
    }
    Serial.println(dataep);
    Serial.println(reasonep);
    Serial.println(calibrationFactor);
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

void postData(String postStr)
{
  WiFiClient client;
  HTTPClient http;

  http.begin(client, postdata);

  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(postStr);

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  // Free resources
  http.end();
}