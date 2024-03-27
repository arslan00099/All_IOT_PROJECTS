#include <WiFi.h>
#include <HTTPClient.h>
#include <FS.h>
#include <Arduino.h>
#include <Preferences.h>

void getDataFromServer() {

  HTTPClient http;
  http.begin("http://espstack.com/pump/fingerprintSideGetApi.php");
  int httpResponseCode = http.GET();

  if (httpResponseCode == HTTP_CODE_OK) {
    String jsonData = http.getString();
    Serial.println("DATA RESPONSE : "+(String)jsonData);

  } else {
    Serial.print("HTTP GET request failed with error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();

}
