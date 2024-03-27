#include <HardwareSerial.h>

HardwareSerial HC12Serial(1);  // Use Serial2 for HC-12 communication

void setup() {
  Serial.begin(115200);          // Initialize serial communication for debugging
  HC12Serial.begin(9600, SERIAL_8N1, 35, 22);  // Initialize HC-12 module communication (RX: GPIO35, TX: GPIO32)
}

void loop() {
  if (HC12Serial.available()) {
    String message = HC12Serial.readString();  // Read the received message
    Serial.println("Received message: " + message);
  }
}
