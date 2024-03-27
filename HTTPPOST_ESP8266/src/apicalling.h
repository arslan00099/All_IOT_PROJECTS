#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "Neo";
const char* password = "12345678";
const char* serverName = "http://espstack.com/hitapi.php";

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

void setupWifi() {

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 }

void sendHttp(String sendtoServer) {
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);
  
     
      
      // If you need an HTTP request with a content type: application/json, use the following:
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST(sendtoServer);


     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
       if (httpResponseCode == 200) {
        // You can handle the response data here if needed
        String response = http.getString();
        Serial.print("Response: ");
        Serial.println(response);
      }else {
        Serial.println("NOT GETTIG RESPOSE");
      }
        
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}