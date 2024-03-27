
#include <Arduino.h>
#include <DWIN.h>
#include "rfidscanner.h"
#define ADDRESS_A     "1010"
#define ADDRESS_B     "1020"

#define DGUS_BAUD     115200

// If Using ESP 32
#if defined(ESP32)
  #define DGUS_SERIAL Serial2
  DWIN hmi(DGUS_SERIAL, 16, 17, DGUS_BAUD);

// If Using Arduino Uno
#else
  DWIN hmi(2, 3, DGUS_BAUD);
#endif

// Event Occurs when response comes from HMI
void onHMIEvent(String address, int lastByte, String message, String response){  
  Serial.println("OnEvent : [ A : " + address + " | D : "+ String(lastByte, HEX)+ " | M : "+message+" | R : "+response+ " ]"); 
  if (address == "1002"){
  // Take your custom action call
  }
}

void clearData(int address){
 // hmi.beepHMI();
  hmi.setText(address,"                                                                                       ");
    hmi.setText(address,"                                                                                    ");
}



void setup() {
    Serial.begin(115200);
    setupRfid();
    Serial.println("DWIN HMI ~ Hello World");
    hmi.echoEnabled(false);
    hmi.hmiCallBack(onHMIEvent);
    hmi.setPage(0);
    hmi.setText(0X1000,"  PEPSI              160                 1                160");
    hmi.setText(0X2000,"  LAYS               10                  2                20");
    hmi.setText(0X3000,"  CHIPS              60                  1                60");
    hmi.setText(0X4000,"  PIZA               1000                1                1000");
    hmi.setText(0X5000,"  SOAP               60                  1                60");
    hmi.setText(0X6000,"  WATER              100                 1                100");
    hmi.setText(0X7000,"  SURF               60                  2                120");
    hmi.setText(0X8000,"  DISH WASH          100                 1                100");
    hmi.setText(0X9000,"  LEMON SODA         60                  1                60");

    Serial.println("PRINTED");
    delay(5000);
    hmi.setText(0X1000,"                                                                                ");
    hmi.setText(0X1100,"9");
    hmi.setText(0X1200,"1680");
}



void loop() {
    // Listen HMI Events
    hmi.listen();
    readRFID();
    if(rfidSt.length() > 0){

      clearData(0X100);
      if(rfidSt.indexOf("16011518932")>-1){
 hmi.setText(0X1000," LAYS               10                  2                 200");

 }
 else{
 hmi.setText(0X1000," SURF               20                  1                 10 ");
 }
 rfidSt="";
    }
    
}