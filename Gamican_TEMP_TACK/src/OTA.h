
#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <WiFiClientSecure.h>
//#include <cert.h>

boolean  firmFlag=false;
String FirmwareVer = {
  "1.0"// While loop eleminated from wifi connect. Onboard wifi indication
};
#define URL_fw_Version "http://espstack.com/OTA/version.txt"
#define URL_fw_Bin "http://espstack.com/OTA/firmware.bin"



//#define URL_fw_Version "http://cade-make.000webhostapp.com/version.txt"
//#define URL_fw_Bin "http://cade-make.000webhostapp.com/firmware.bin"

void connect_wifi();
void firmwareUpdate();
int FirmwareVersionCheck();



void CHK_FIRMWARE() {


    // save the last time you blinked the LED

    if (FirmwareVersionCheck()) {
     
 firmFlag=true;
    }
  }

 
 
  




void setupOTA() {

  Serial.print("Active firmware version:");
  Serial.println(FirmwareVer);
CHK_FIRMWARE();

if( firmFlag==true)
{
 firmwareUpdate();
}
}








void firmwareUpdate(void) {
  WiFiClient client;
  httpUpdate.setLedPin(2, LOW);
  Serial.println("Updating Firmware");
  t_httpUpdate_return ret = httpUpdate.update(client, URL_fw_Bin);

  switch (ret) {
  case HTTP_UPDATE_FAILED:
    Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
    ESP.restart();
    break;

  case HTTP_UPDATE_NO_UPDATES:
    Serial.println("HTTP_UPDATE_NO_UPDATES");
       ESP.restart();
    break;

  case HTTP_UPDATE_OK:
    Serial.println("HTTP_UPDATE_OK");
       ESP.restart();
    break;
  }
}
int FirmwareVersionCheck(void) {



  if((WiFi.isConnected() == true)) {

        HTTPClient http;

        Serial.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS
        http.begin(URL_fw_Version); //HTTP

        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                Serial.println(payload);

if (payload.equals(FirmwareVer)) {
      Serial.printf("\nDevice already on latest firmware version:%s\n", FirmwareVer);
      return 0;
    } 
    else 
    {
      Serial.println(payload);
      Serial.println("New firmware detected");
      return 1;
    }
  }      
    }
 


}
 return 0;  
}