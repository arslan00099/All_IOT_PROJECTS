#include <Arduino.h>
#include <SoftwareSerial.h>
#include "apicalling.h"
#define MODEM_RX 2
#define MODEM_TX 3

SoftwareSerial SerialAT(3, 2);
String sendtoserver;

String gsm_send_serial(String command, int delay)
{
  String buff_resp = "";
  Serial.println("Send ->: " + command);
  SerialAT.println(command);
  long wtimer = millis();
  while (wtimer + delay > millis())
  {
    if (SerialAT.available())
    {
      buff_resp = SerialAT.readString();
      Serial.println(buff_resp);
    }
  }
  Serial.println();
  return buff_resp;
}


void postdata()
{
  Serial.println("###### POSTING DATA #######");
  gsm_send_serial("AT+HTTPINIT", 2000);
  gsm_send_serial("AT+HTTPPARA=\"CID\",1", 1000);
  gsm_send_serial("AT+HTTPPARA=\"URL\",\"https://api.ng.termii.com/api/sms/send\"", 1000);  // write server code here
  gsm_send_serial("AT+HTTPPARA=\"CONTENT\",\"application/json\"", 1000);
  gsm_send_serial("AT+HTTPSSL=1", 1000);
  gsm_send_serial("AT+HTTPSSL?", 1000);
  String CSQ = gsm_send_serial("AT+CSQ", 300);
  String SQ = CSQ.substring(CSQ.indexOf(":") + 2, CSQ.indexOf(","));
  Serial.println("Signal Quality" + SQ);
 
  sendtoserver = "{\"to\":\"00923415897489\",\"from\":\"Skilller\",\"sms\":\"Hithere,testingTermii\",\"type\":\"plain\",\"channel\":\"generic\",\"api_key\":\"TLlfVq8VJCKwBzQI8cSjZFD53T1yqazRYRQwoKqNNDdNodnXlxyJZAJ1XUxQBN\"}";
  gsm_send_serial(" AT + HTTPDATA = " + String(sendtoserver.length()) + ", 100000 ", 1000);
  gsm_send_serial(sendtoserver, 1000);
  gsm_send_serial("AT+HTTPACTION=1", 1000);
  gsm_send_serial("AT+HTTPREAD", 3000);
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    SerialAT.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(SerialAT.available()) 
  {
    Serial.write(SerialAT.read());//Forward what Software Serial received to Serial Port
  }
}




void setup() {
  Serial.begin(115200);
 SerialAT.begin(9600);
 Serial.println("Initializing...");
  delay(1000);
  setupWifi();

  // SerialAT.println("AT"); //Once the handshake test is successful, it will back to OK
  // updateSerial();
  // SerialAT.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  // updateSerial();
  // SerialAT.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  // updateSerial();
  // SerialAT.println("AT+CREG?"); //Check whether it has registered in the network
  // updateSerial();

  
}

void loop() {
  //postdata();
  sendtoserver = "{\"to\":\"2349123766999\",\"from\":\"Skilller\",\"sms\":\"Hithere,testingTermii\",\"type\":\"plain\",\"channel\":\"generic\",\"api_key\":\"TLlfVq8VJCKwBzQI8cSjZFD53T1yqazRYRQwoKqNNDdNodnXlxyJZAJ1XUxQBN\"}";
sendHttp(sendtoserver);

}

