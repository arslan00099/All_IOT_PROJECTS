#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define LED_PIN 2


const char* ssid = "Islo AI";
const char* password = "gamican3506";
unsigned long premillis=0;

const char *serverName = "http://espstack.com/new/getapi.php";

void getData()
{
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);

  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST("{\"led\":\"2\"}");
  Serial.print("HTTP GET Response code FOR GETAPI : ");
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0)
  {
    String payload = http.getString();
    Serial.println("PAYLOAD : "+(String)payload);
    if(payload.indexOf("1")>-1){
      digitalWrite(LED_PIN,HIGH);
      Serial.println("digital Write high");
    }

      if(payload.indexOf("0")>-1){
      digitalWrite(LED_PIN,LOW);
       Serial.println("digital Write LOW");
    }
   
    
  }

  http.end();
}

void setup(){
 Serial.begin(115200);
 pinMode(LED_PIN,OUTPUT);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  

  Serial.println("CONNECTED");
   
  digitalWrite(LED_PIN,HIGH);
  delay(500);
  digitalWrite(LED_PIN,LOW);
  delay(500);
}

void loop(){

  if(millis()- premillis >5000){
    getData();
    premillis = millis();
  }


}

