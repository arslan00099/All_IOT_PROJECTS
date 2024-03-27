// see http://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
#include <Arduino.h>
#define PRINT_DELAY 60 // seconds
#include <sys/time.h>
#include <Ticker.h>
#include "time.h"
#include <WiFiUdp.h>
#include <time.h>
#include <TimeLib.h>




Ticker time_print;
boolean validTime;
String now_Day_name = "05";
String now_Day = "05";
String now_Month = "22";
String now_Year = "04";
String now_Hour = "08";
String now_Minute = "22";
String now_Second = "00";
String now_month_alp="";
const char *ntpServer = "europe.pool.ntp.org";

// Variable to save current epoch time
unsigned long epochTime;
/* This function sets the internal time. The timeval
   struct is seconds, milliseconds. settimeofday parameters are
   epoch_time, timezone.
*/
int setUnixtime(int32_t unixtime)
{
  timeval epoch = {unixtime, 0};
  return settimeofday((const timeval *)&epoch, 0);
}

void timePrint()
{
  struct tm now;
  getLocalTime(&now, 0);
  if (now.tm_year >= 117)
  {
    validTime = true;
    Serial.println(&now, "%B %d %Y %H:%M:%S (%A)");

    if (now.tm_hour < 10)
    {
      now_Hour = "0" + (String)now.tm_hour;
    }
    else
    {
      now_Hour = (String)now.tm_hour;
    }

    if (now.tm_min < 10)
    {
      now_Minute = "0" + (String)now.tm_min;
    }
    else
    {
      now_Minute = (String)now.tm_min;
    }

    if (now.tm_sec < 10)
    {
      now_Second = "0" + (String)now.tm_sec;
    }
    else
    {
      now_Second = (String)now.tm_sec;
    }

    if (now.tm_mday < 10)
    {
      now_Day = "0" + (String)now.tm_mday;
    }
    else
    {
      now_Day = (String)now.tm_mday;
    }

    if (now.tm_mon < 10)
    {
      now_Month = "0" + (String)(now.tm_mon + 1);
    }
    else
    {
      now_Month = (String)(now.tm_mon + 1);
    }

    now_Year = now.tm_year + 1900;
    now_month_alp=now.tm_mday;
  }
  else
  {
    validTime = false;
    ESP.restart();
  }
}

unsigned long getTime()
{
  time_t now;
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return (0);
  }
  time(&now);
  return now;
}



