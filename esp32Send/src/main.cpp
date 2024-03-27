#include <Arduino.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <Wire.h>
#include <SPIFFS.h>

const char* ssid     = "NCM";
const char* password = "NemaKabla14";
const char* serverName = "http://192.168.100.6/tmonitor/post-esp-data.php";
String apiKeyValue = "tPmAT5Ab3j7F9";
String deviceid = "1"; //ovde unesite id(indetifikacioni broj) uredjaja moraju biti samo brojevi
#define SS_PIN 5
#define RST_PIN 22
MFRC522 mfrc522(SS_PIN, RST_PIN);  
int readsuccess;
byte readcard[4];
char str[32] = "";
String StrUID;
String osoba;


int postData(String postStr)
{
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);

  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(postStr);
Serial.println("HTTP POST: "+(String)postStr);
  Serial.print("HTTP POST DATA Response code: ");
  Serial.println(httpResponseCode);

  // Free resources
  http.end();
  return httpResponseCode;
}
void array_to_string(byte array[], unsigned int len, char buffer[]){
    for (unsigned int i = 0; i < len; i++)
    {
        byte nib1 = (array[i] >> 4) & 0x0F;
        byte nib2 = (array[i] >> 0) & 0x0F;
        buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
        buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
    }
    buffer[len*2] = '\0';
}

int getid(){  
  if(!mfrc522.PICC_IsNewCardPresent()){
    return 0;
  }
  if(!mfrc522.PICC_ReadCardSerial()){
    return 0;
  }

  for(int i=0;i<4;i++){
    readcard[i]=mfrc522.uid.uidByte[i]; //storing the UID of the tag in readcard
    array_to_string(readcard, 4, str);
    StrUID = str;
  }
  mfrc522.PICC_HaltA();
  return 1;
}



void setup() {
  pinMode(4, OUTPUT);  
  pinMode(17, OUTPUT);  
  digitalWrite(4,HIGH);
  digitalWrite(17,HIGH);
  delay(500);
  digitalWrite(4,LOW);
  digitalWrite(17,LOW);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting"); 
  while(WiFi.status() != WL_CONNECTED) { 
    digitalWrite(17,HIGH);
    delay(250);
    Serial.print(".");
    digitalWrite(17,LOW);
    delay(250);
  }
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
String jsondata="{\"id\":\"1\",\"rfid\":\"5678\",\"osoba\":\"Dragon\"}";
int response=postData(jsondata);

}

void loop() {
  while(WiFi.status() != WL_CONNECTED) { 
    ESP.restart();
    digitalWrite(17,HIGH);
    delay(250);
    Serial.print(".");
    digitalWrite(17,LOW);
    delay(250);
  }
  readsuccess = getid();
  if(WiFi.status()== WL_CONNECTED){
    digitalWrite(17,HIGH);
    if(readsuccess){
      String osobaCheck = StrUID;
      if(osobaCheck == "2C154563"){
        osoba = "1";
      }
      if(osobaCheck == "2C56636D"){
        osoba = "2";
      }
      if(osobaCheck == "2C20636D"){
        osoba = "3";
      }
      if(osobaCheck == "DCEC846D"){
        osoba = "4";
      }
      if(osobaCheck == "9C399C64"){
        osoba = "5";
      }

String jsondata="{\"id\":\"1\",\"rfid\":\"5678\",\"osoba\":\"Dragon\"}";
int response=postData(jsondata);
      if (response>0) {
        Serial.print("HTTP Response code: ");
       
        digitalWrite(4, HIGH);
        delay(500);
        digitalWrite(4, LOW);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(response);
      }
      
  }
}
else{
    digitalWrite(17,LOW);
  }
}
