#define STATUS_RED 27
#define STATUS_GREEN 26
#define STATUS_BLUE 25
#include <Arduino.h>
void ALL_OFF();
void RESET_CREDENTIALS()
{
    ALL_OFF();
    analogWrite(STATUS_RED, 200);
    //digitalWrite(STATUS_GREEN, 200);

}
void ALL_OFF()
{
    digitalWrite(STATUS_GREEN, 255);
    digitalWrite(STATUS_BLUE, 255);
    digitalWrite(STATUS_RED, 255);
}
void WIFI_MISSING()
{
     ALL_OFF();
digitalWrite(STATUS_BLUE, 200);
}

void WIFI_CONNECT()
{
       ALL_OFF();
    digitalWrite(STATUS_RED, 200);
    //digitalWrite(STATUS_GREEN, 30);
}
void WIFI_POST()
{
      ALL_OFF();
    analogWrite(STATUS_BLUE, 200);
    delay(500);
}