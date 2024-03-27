// https://api.coingecko.com/api/v3/coins/list
// http://www.rinkydinkelectronics.com/t_imageconverter565.php
#include <Arduino.h>
#include "WiFi.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "ledPanel.h"
#include "config.h"
#include "SPIFFS.h"
#include "NTPClient.h"
#include "RTC_ULP.h"
#include "ntp.h"

unsigned long prevMillis = 0;
unsigned long prevMilliConfig = 0;
const char *nameOfCoin;
boolean flag=false;
String msg="I LOVE PAKISTAN";
int scrol=64;
int delayms=100;


void setup()
{

  Serial.begin(115200);
   setupPanel();
//   delay(1000); // Take some time to open up the Serial Monitor
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
               " Seconds");
//   // Print the wakeup reason for ESP32
   print_wakeup_reason();

  /*
  First we configure the wake up source
  We set our ESP32 to wake up every 5 seconds
  */
 

  /* code */
  //loopNTP();

  // esp_deep_sleep_start();
  // Serial.println("This will never be printed");
}

void loop()
{

  if (millis() - prevMillis >= (1000-delayms))
  {
    getTime();
    timePrint();
    Serial.println("Going to sleep now");
       clearMatrix();
         String dat = now_Day + "-" + now_Month + "-" + now_Year;
    displayMatrix(2, 10, 1, matrix.Color333(7, 4, 0), dat);

    if(flag==false)
    {
    String S = now_Hour + ":" + now_Minute + ":" + now_Second;
    
    displayMatrix(8, 1, 1, matrix.Color333(0, 7, 0), S);
    }
    else
    {
        String S = now_Hour + " " + now_Minute + " " + now_Second;
    displayMatrix(8, 1, 1, matrix.Color333(0, 7, 0), S);
    }

    prevMillis = millis();
    flag=!flag;
  }
  
if(msg.length()>0)
{
    displayMatrix(scrol, 18, 2, matrix.Color333(7, 0, 0), msg);
scrol--;
if(scrol== -(64 + msg.length() * 7))
scrol=64;
delay(100);
matrix.fillRect(0,18,64,18,matrix.Color333(0, 0, 0));

}
  
  if (SerialBT.available()) {
    //Serial.write(SerialBT.read());
  msg=SerialBT.readString();
  }

  
}