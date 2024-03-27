#include <Arduino.h>
#include <time.h>
#include <sntp.h>
#include "my_clock.h"

void ntpServerSetUp(const char *timeZone) {
  const char* ntpServer1 = "pool.ntp.org";
  const char* ntpServer2 = "time.nist.gov";
  sntp_servermode_dhcp(1);
  configTzTime(timeZone, ntpServer1, ntpServer2); // if Time zone value not found in NVS. Start with default time zone GMT+5 (Pakistan time zone) 
}

void getTimeString(char* timeStrOutput){
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo, 500)) {
        timeStrOutput =  "------";
     }
  else{
       strftime(timeStrOutput, 10, "%I:%M:%.S", &timeinfo);  // %H in 24h format. %I in 12h formate
  }
}
