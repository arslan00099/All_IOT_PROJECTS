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
String station_ID = "2";
String rfidSt = "";
int relayTimeOut;
boolean getReasonFlag=false;

int minkelometers, maxkelometers;
float calibrationFactor;
const int MAX_RECORDS = 500;
String vechileRfidArray[MAX_RECORDS];
String keloArray[MAX_RECORDS];

const char *reasonrequest = "http://espstack.com/pump/getreasonapi.php";
const char *epouch = "http://espstack.com/pump/getepoch.php";
const char *serverName = "http://espstack.com/pump/getapi.php";
const char *postdata = "http://espstack.com/pump/postApi.php";

void writeJsonToSpiff(const char *fileName, const char *jsonStr)
{
  String filePath = String(fileName);

  Serial.println("Saving to SPIFF log FOR "+filePath);
  if (!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS initialization failed");
    return;
  }

  if (SPIFFS.exists(filePath))
  {
    Serial.println("SPIFF FILE EXIST TRYING TO REMOVE IT");
    SPIFFS.remove(filePath);
   
  }
  
  File file = SPIFFS.open(filePath, "w");
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return;
  }

  file.print(jsonStr);
  Serial.println("JSON FILE WRITTEN SUCESSFULLY");
  file.close();

  finalLiters = 0;
  value = 0;
 getReasonFlag=true;
}


void getData()
{
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);

  http.addHeader("Content-Type", "application/json");
  int httpResponseCode =  http.POST("{\"staid\":\"" + station_ID + "\"}");
  Serial.print("HTTP GET Response code FOR GETAPI : ");
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
  int httpResponseCode =  http.POST("{\"staid\":\"" + station_ID + "\"}");

  Serial.print("HTTP GET REASON Response code FOR GETREASONAPI : ");
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

  Serial.print("HTTP GET EPOCH Response code: ");
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
    int timeout= doc["timeout"];
    

    for (int i = 0; i < doc.size() - 5; i++)
    {
      String vechilerfid = doc[String(i)]["vid"];
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
      String payload = "{\"minK\":\"" + (String)minimunV + "\",\"maxK\":\"" + (String)maximumV + "\",\"cf\":\"" + (String)CF + "\",\"timeout\":\"" + (String)timeout + "\"}";
      writeJsonToSpiff("/minMaxKelo.json", payload.c_str());
      minkelometers = minimunV;
      maxkelometers = maximumV;
      calibrationFactor = CF;
      relayTimeOut=timeout;
    }
    Serial.println(dataep);
    Serial.println(reasonep);
    Serial.println(calibrationFactor);
    Serial.println("TIMEOUT : "+(String)timeout);
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

int postData(String postStr)
{
  WiFiClient client;
  HTTPClient http;

  http.begin(client, postdata);

  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(postStr);
Serial.println("HTTP POST: "+(String)postStr);
  Serial.print("HTTP POST DATA Response code: ");
  Serial.println(httpResponseCode);

  // Free resources
  http.end();
  return httpResponseCode;
}