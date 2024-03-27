#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <Wire.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

const char *ssid = "Islo AI";
const char *password = "gamican3506";

void setupWifi()
{
  WiFi.mode(WIFI_STA);
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

void writeJsonToSpiff(const char *fileName, const char *jsonStr)
{
  File file = SPIFFS.open(fileName, "r");

  if (file)
  {
    String existingContent = file.readString();
    file.close();

    DynamicJsonDocument jsonDocument(1024);
    deserializeJson(jsonDocument, existingContent);

    jsonDocument.add(jsonStr);

    file = SPIFFS.open(fileName, "w");

    if (file)
    {
      String updatedContent;
      serializeJson(jsonDocument, updatedContent);

      file.println(updatedContent);

      file.close();
    }
    else
    {
      Serial.println("Failed to open file for writing");
    }
  }
  else
  {
    Serial.println("Failed to open file for reading");
  }
}

String readJsonFromFile(const char *filename)
{
  File file = SPIFFS.open(filename, "r");
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return "";
  }

  String jsonString = file.readString();
  file.close();

  return jsonString;
}

int callPostApi(String postData, String endpoint)
{
  int response;
  WiFiClientSecure *client = new WiFiClientSecure;
  if (client)
  {
    client->setInsecure();
    HTTPClient https;

    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, "https://dashboard.temptracker.net/androidApis/" + endpoint))
    {
      Serial.print("[HTTPS] GET...\n");

      int httpCode = https.POST(postData);
      if (httpCode > 0)
      {

        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {

          String payload = https.getString();
          Serial.println(payload);
        }
      }
      else
      {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end();
      response = httpCode;
    }
  }
  else
  {
    Serial.printf("[HTTPS] Unable to connect\n");
    response = 400;
  }
  return response;
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
  Serial.println("DONE");
  delay(50000);
}

void listMySpiff()
{
  Serial.println("######################  LISTING SPIFF #######################");
  String fileName = "/upload.json";

  File file = SPIFFS.open(fileName, "r");

  if (file)
  {
    String fileContent = file.readString();

    file.close();

    Serial.println("File Content:");
    Serial.println(fileContent);
  }
  else
  {
    Serial.println("Failed to open file for reading");
  }
  Serial.println("######################  END SPIFF #######################");
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  if (!SPIFFS.begin(true))
  {
    Serial.println("Failed to mount file system");
    return;
  }
  else
  {
    Serial.println("SPIFF MOUNTED");
    // resetSPIFFS();
    listMySpiff();
  }

  setupWifi();
  Serial.println(WiFi.localIP());
  String postString = "{\"device_id\":\"DEV4\",\"email\":\"ArslanTest@gmail.com\",\"temp\":99,\"humi\":99,\"epoch\":\"1699442342\",\"ssid\":\"Gamicann\",\"rssi\":20,\"battery_life\":55,\"version\":1.1}";

  int res = callPostApi(postString, "post.php");
  Serial.println("RESPOSE :" + (String)res);
  if (res != HTTP_CODE_OK)
  {
    String fileName = "/upload.json";
    Serial.println(fileName);
    writeJsonToSpiff(fileName.c_str(), postString.c_str());
  }

  String jsonData = readJsonFromFile("/upload.json");
  if (jsonData.length() > 0)
  {
    int httpCode = callPostApi(jsonData, "postList.php");

    if (httpCode == HTTP_CODE_OK)
    {
      if (SPIFFS.remove("/upload.json"))
      {
        Serial.println("File '/upload.json' removed successfully");
      }
      else
      {
        Serial.println("Failed to remove file '/upload.json'");
      }
    }
    else
    {
      Serial.println("POST failed");
    }
  }
}

void loop()
{
}