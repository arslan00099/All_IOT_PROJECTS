#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "myntp.h"
#define bulb_PIN 19
boolean heatflag=false;


const char *ssid = "RIGJAZZ";
const char *password = "Kashmiri786";

const char *serverName = "http://espstack.com/petfeeder/getapi.php?id=testfeeder";
unsigned long lastTime = 0;
unsigned long timerDelay = 1000*10;

String sensorReadings;
float sensorReadingsArr[3];
String payload = "{}";

unsigned premillis=0;
int currentWeight=0;

String httpGETRequest(const char *serverName)
{
  WiFiClient client;
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);


  int httpResponseCode = http.GET();



  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
    Serial.println((String)payload);
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void setup()
{
  Serial.begin(115200);
  pinMode(bulb_PIN,OUTPUT);
  digitalWrite(bulb_PIN,LOW);
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

while (epochTime < 1){
  getntp();
}
Serial.println(rtc.getTime());
  }

void loop()
{
  if(millis()-premillis > 1000){
  Serial.println(rtc.getTime());
  premillis=millis();
  }


  if ((millis() - lastTime) > timerDelay)
  {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {

      sensorReadings = httpGETRequest(serverName);
     // Serial.println((String)sensorReadings);
      DynamicJsonDocument doc(2500);
  DeserializationError error = deserializeJson(doc, payload);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

for(int i=0;i<4;i++){
  Serial.println("##################"+(String)i+"###############");
  String scheduleName  = doc[i]["schedulename"];
  String starttime  = doc[i]["stime"];
  int weight = doc[i]["weight"];
    String heatTime  = doc[i]["htime"];
  String timer= doc[i]["timer"];
  String hstatus=doc[i]["hstatus"];

   Serial.println((String)scheduleName);
      Serial.println((String)starttime);
         Serial.println((String)weight);
            Serial.println((String)timer);
              Serial.println((String)hstatus);

                     String h_hour = heatTime.substring(0,2);
               if(h_hour[0]== 0){
                 h_hour = heatTime.substring(1,2); 
               }
               String h_min = heatTime.substring(3,5);
                 if(h_min[0]== 0){
                 h_min = heatTime.substring(4,5); 
               }
             
               int heat_hour=h_hour.toInt();
               int heat_min=h_min.toInt();
               
    if((heat_hour == rtc.getHour(true)) && (heat_min > rtc.getMinute()) && (hstatus.indexOf("ON") > -1)){
     heatflag=true;
    }
    else{ heatflag=false;}

               String s_hour = starttime.substring(0,2);
               if(s_hour[0]== 0){
                 s_hour = starttime.substring(1,2); 
               }
               String s_min = starttime.substring(3,5);
                 if(s_min[0]== 0){
                 s_min = starttime.substring(4,5); 
               }
             
               int start_hour=s_hour.toInt();
               int start_min=s_min.toInt();

    if((start_hour == rtc.getHour(true)) && (start_min > rtc.getMinute())){
      Serial.println("TURN ON MOTOR");
      delay(15000);
    }
    
    Serial.println(start_hour);
      Serial.println(start_min);
            
}
     
}
       lastTime=millis();
   
    }  

    if(heatflag == true){
      digitalWrite(bulb_PIN,HIGH);
    }
    else{
      digitalWrite(bulb_PIN,LOW);
    }
}
