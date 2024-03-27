#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "RIGJAZZ";
const char* password = "Kashmiri786";
const char* host = "espstack.com";
#define  led_PIN 2
unsigned long premillis=0;

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(led_PIN,OUTPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

 
}

void loop() {

if(millis()- premillis >1000){
   WiFiClient client;
  const int httpPort = 80;

  Serial.print("Connecting to ");
  Serial.println(host);

  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }

  Serial.print("Requesting URL: ");
  client.print(String("GET /getapi.php HTTP/1.1\r\n") +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("Request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');

    Serial.println(line);

    if(line.indexOf("on")>-1){
      digitalWrite(led_PIN,LOW);
    }
    if(line.indexOf("off")>-1){
      digitalWrite(led_PIN,HIGH);
    }
  }
  Serial.println("Response received");

  premillis=millis();
}

}
