#include <Arduino.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
 String json = "{\"a\":\"1000\",\"b\":\"2000\",\"c\":\"3000\",\"d\":\"4000\"}";

// Convert the String to a byte array
const char* jsonString = json.c_str();

// Calculate the length of the string
size_t jsonLength = strlen(jsonString);

// Send the byte array over Bluetooth
SerialBT.write((uint8_t*)jsonString, jsonLength);
  delay(15000);
}