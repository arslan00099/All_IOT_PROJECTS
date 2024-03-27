#include <Arduino.h>
#include <WiFi.h>
#include "test.pb.h"
#include <pb_encode.h>
#include <pb_decode.h>
#include <SPIFFS.h>
#include <Arduino.h>
#include <SPIFFS.h>
#include "test.pb.h" // Include the generated C++ header for settings.proto

void setup() {
  Serial.begin(115200);
  SPIFFS.begin();

  // Create a DeviceSettings message
  DeviceSettings settings;
  settings.deviceid("hello"); // Replace "your_device_id_here" with the actual device ID

  // Serialize the message to binary data
  std::string serializedData;
  settings.SerializeToString(&serializedData);

  // Save the binary data to the "setting.bin" file in SPIFFS
  File file = SPIFFS.open("/setting.bin", "w");
  if (file) {
    file.write((uint8_t*)serializedData.c_str(), serializedData.length());
    file.close();
    Serial.println("Device ID saved to setting.bin.");
  } else {
    Serial.println("Failed to open setting.bin for writing.");
  }
}

void loop() {
  // Your loop code here
}

