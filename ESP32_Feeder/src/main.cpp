#include "fbcheck.h"
#include "HX711.h"
#include "soc/rtc.h"
#include <Servo.h>
#include <WiFi.h>
#include "time.h"
#include <ESP32Time.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define bulb_PIN 19
boolean heatflag=false;
float reading;

ESP32Time rtc(3600);  // offset in seconds GMT+1

const char *ssid = "Neo";
const char *password = "12345678";
String feedername="noor";

String serverName = "http://remanent-hours.000webhostapp.com/getapi.php";
String serverNotify = "http://remanent-hours.000webhostapp.com/notify.php";
String to="eQa6H1IQQb25EY6h3G0irB:APA91bEIGtQBdORWjfw0olXRIRHBXydZ5qp0w5yh-robAWg4ttiTtlxvVoxsBwIdK0EVjE_NihJyZdBuuxYOTaoi_mV48XAyGhsCTAQQy9xPlxFnF4IDd8eTl_j59Q2Rt4QpzOaB621J";
unsigned long lastTime = 0;
unsigned long timerDelay = 1000 * 5;

String sensorReadings;
float sensorReadingsArr[3];
String payload = "{}";
unsigned long premillislight;

unsigned premillis=0;
int currentWeight=0;

const char* ntpServer = "europe.pool.ntp.org";
const long  gmtOffset_sec = 3600 * 4;
const int   daylightOffset_sec = 0;
// Variable to save current epoch time
unsigned long epochTime=0; 

Servo myservo;
int pos = 0;

const int LOADCELL_DOUT_PIN = 16;
const int LOADCELL_SCK_PIN = 4;

HX711 scale;

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}

String httpGETRequest(const char *serverName)
{Serial.println("-----------CALLING API--------------");
  WiFiClient client;
  HTTPClient http;
    http.begin(client, serverName);
reading=round(scale.get_units());
 http.addHeader("Content-Type", "application/json");
 String sendtoServer="{\"id\":\""+feedername+"\",\"reading\":\""+(String)reading+"\"}";
  int httpResponseCode = http.POST(sendtoServer);
  Serial.println(sendtoServer);

 Serial.print("HTTP Response code: ");
  //Serial.println(httpResponseCode);
  // Your Domain name with URL path or IP address with path


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


String notify(const char *serverNotify,String title,String des)
{Serial.println("-----------Sending Notification--------------");
  WiFiClient client;
  HTTPClient http;
    http.begin(client, serverNotify);
reading=round(scale.get_units());
 http.addHeader("Content-Type", "application/json");
 String sendtoServer="{\"title\":\""+(String)title+"\",\"des\":\""+(String)des+"\",\"to\":\""+(String)to+"\"}";
  int httpResponseCode = http.POST(sendtoServer);
  Serial.println(sendtoServer);

 Serial.print("HTTP Response code: ");

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
void getntp() {

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  epochTime = getTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);
  rtc.setTime(epochTime);

}

void setupScale(){
  
  Serial.println("Initializing the scale");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());      // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));   // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
            // by the SCALE parameter (not set yet)
            
  scale.set_scale(458);
  //scale.set_scale(-471.497);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
            // by the SCALE parameter set with set_scale

  Serial.println("################# DONE SETUP ###################");


}

void setup() {
  Serial.begin(9600);
  Serial.println("STARTED");
 pinMode(bulb_PIN,OUTPUT);

  //setupScale();
   myservo.attach(13);
myservo.write(30); // on 30 door is closed and on 120 it is opened




    pinMode(bulb_PIN,OUTPUT);
  digitalWrite(bulb_PIN,LOW);
 Serial.println("Initializing WiFi...");
  WiFi.mode(WIFI_STA);  Serial.println("Connecting to WiFi ");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
    String sendnotifiaction = notify(serverNotify.c_str(),"petfeeder","your petfeeder is online");
setupfirebase();

while (epochTime < 1){
  getntp();
}
Serial.println(rtc.getTime());
premillislight=0;
  }



void loop() {
   if(millis()-premillis > 1000){
  Serial.println("ESP TIME : "+(String)rtc.getTime());
     reading=round(scale.get_units());
     Serial.println((String)heatflag);
  Serial.println("WEIGHT :"+(String)reading);
  premillis=millis();
  }


  if ((millis() - lastTime) > timerDelay)
  {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {

      sensorReadings = httpGETRequest(serverName.c_str());
    
     // Serial.println((String)sensorReadings);
      DynamicJsonDocument doc(2500);
  DeserializationError error = deserializeJson(doc, payload);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

for(int i=0;i<doc.size();i++){
  Serial.println("##################"+(String)i+"###############");
  String scheduleName  = doc[i]["schedulename"];
  String starttime  = doc[i]["stime"];
  int weight = doc[i]["weightt"];
    String heatTime  = doc[i]["htime"];
  int timer= doc[i]["timer"];
  String hstatus=doc[i]["hstatus"];

   Serial.println("SCHEDULE NAME : "+(String)scheduleName);
      Serial.println("START TIME : "+(String)starttime);
      //Serial.println("ESP TIME : "+(String)rtc.getHour(true)+"::"+(String)rtc.getMinute());
         Serial.println("WEIGHT : "+(String)weight);
            Serial.println("HEAT TIMER : "+(String)timer);
              Serial.println("HEAT STATUS : "+(String)hstatus);

                     String h_hour = heatTime.substring(0,2);
                     //  Serial.println(h_hour);
               if(h_hour[0]== 0){
                 h_hour = heatTime.substring(1,2); 
               
               }
               String h_min = heatTime.substring(3,5);
                 if(h_min[0]== 0){
                 h_min = heatTime.substring(4,5); 
               }
               
             
               int heat_hour=h_hour.toInt();
               int heat_min=h_min.toInt();
              // Serial.println((String)heat_hour+":::"+(String)heat_min);
               
    if((heat_hour == rtc.getHour(true)) && ( rtc.getMinute() >= heat_min) && (heatflag==false)){
      Serial.println("BULB ON");
      digitalWrite(bulb_PIN,HIGH);
      premillislight=0;
     while ( millis() - premillislight < timer*60000){
  
     }
     heatflag=true; 
      digitalWrite(bulb_PIN,LOW);
    }
    else{ 
     digitalWrite(bulb_PIN,LOW);}

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

    if((start_hour == rtc.getHour(true)) && (rtc.getMinute() >= start_min ) && (rtc.getMinute() <= start_min+1 )){
      
      reading=round(scale.get_units());
      
      while (reading < weight){
      Serial.println("MOTOR ON");
       myservo.write(120);
       reading=round(scale.get_units()); 
     
      }
      // Serial.println((String)reading);
      // Serial.println((String)weight);
     // delay(5000);
       myservo.write(30);
       
      
    }
    
    // Serial.println(start_hour);
    //   Serial.println(start_min);
            
}
     
}
       lastTime=millis();
   
    }  

readfbStatus();
}