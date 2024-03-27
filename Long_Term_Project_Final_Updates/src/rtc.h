
#include <Wire.h>
#include "RTClib.h"
DateTime now ;
String dateStamp2;
String timeStamp2;
RTC_DS3231 rtc2; // RTC object

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
#define RTC_SCL_PIN 33    // Custom SCL pin number VN
#define RTC_SDA_PIN 32    // Custom SDA pin number VP

void setDateTimeFromEpoch(uint32_t epochTime) {
  DateTime dt = DateTime(epochTime);
  
  rtc2.begin();
  rtc2.adjust(dt);

  // Uncomment the line below if you want to enable the 32kHz output pin
  // rtc.writeSqwPinMode(DS3231_SquareWave1Hz);

  // Uncomment the line below if you want to enable the alarm interrupts
  // rtc.enableAlarm(rtc.MATCH_HHMMSS);
}
void setupRTC() {
 
   Wire.begin(RTC_SDA_PIN, RTC_SCL_PIN);   // Initialize I2C communication with custom pins

  rtc2.begin(); // start RTC communication
 // server.begin(); // start server
    if (! rtc2.begin()) {
    Serial.println("Couldn't find RTC");
  
  }


}
void showRTCdT()
{
 now = rtc2.now();

    // Serial.print(now.year(), DEC);
    // Serial.print('/');
    // Serial.print(now.month(), DEC);
    // Serial.print('/');
    // Serial.print(now.day(), DEC);
    // Serial.print(" (");
    // Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    // Serial.print(") ");
    // Serial.print(now.hour(), DEC);
    // Serial.print(':');
    // Serial.print(now.minute(), DEC);
    // Serial.print(':');
    // Serial.print(now.second(), DEC);
    // Serial.println();

    // Serial.print(" since midnight 1/1/1970 = ");
    // Serial.print(now.unixtime());
    // Serial.print("s = ");
    // Serial.print(now.unixtime() / 86400L);
    // Serial.println("d");

    Serial.print("Temperature: ");
    Serial.print(rtc2.getTemperature());
    Serial.println(" C");

    Serial.println();

    dateStamp2=(String)now.year()+"-"+(String)now.month()+"-"+(String)now.day();
Serial.println("DateStamp: "+(dateStamp2));

timeStamp2=(String)now.hour()+":"+(String)now.minute()+":"+(String)now.second();
Serial.println("TimeStamp: "+(timeStamp2));
   //delay(2000);
}

uint64_t getEpochTimeMillis() {

  uint64_t epochTimeMillis = now.unixtime();
//   epochTimeMillis += now.;
  return epochTimeMillis;
}