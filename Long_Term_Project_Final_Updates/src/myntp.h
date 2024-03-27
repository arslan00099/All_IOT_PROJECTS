#include <WiFi.h>
#include "time.h"
#include <ESP32Time.h>

// ESP32Time rtc;
ESP32Time rtc(3600); // offset in seconds GMT+1

// NTP server to request epoch time
const char *ntpServer = "europe.pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;
// Variable to save current epoch time
unsigned long epochTime = 0;

// Function that gets current epoch time
unsigned long getTime()
{
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    // Serial.println("Failed to obtain time");
    return (0);
  }
  time(&now);
  return now;
}

void getntp()
{

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  epochTime = getTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);
  rtc.setTime(epochTime);
}