#include <Arduino.h>
#include <settings.h>
#include <settings.pb.h>

void setup() {

  Serial.begin(9600);
  
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  // Check if the settings.bin file exists
  if (!LittleFS.exists("/settings.bin")) {
    // Settings file doesn't exist, create default settings
    createDefaultSettings(); //To be implemented
  }
  else {
    Serial.println("It exists");
  }

  Settings device_settings;
/* 
  
  if (loadSettings(device_settings)) //To be implemented
  {
      Serial.println("Devicename");

 // Serial.printf("Device Name:", device_settings.boot_sequence);
  //Serial.printf("SSID: %s", device_settings.soft_ap_config.SSID);
  //Serial.printf("Password: %s", device_settings.soft_ap_config.password);

 // Serial.printf("Password: %s", device_settings.device_name);
  }
 */
}

void loop() {
  // nothing
}
