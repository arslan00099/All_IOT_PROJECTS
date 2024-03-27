#include <Arduino.h>
const int resetPin = 2;  // Replace with the GPIO pin you want to use for reset
unsigned long resetDelay = 5000;  // Delay in milliseconds (5 seconds in this example)
unsigned long resetStartTime;

void setup() {
  Serial.begin(115200);
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, LOW); // Set the pin low initially
  resetStartTime = millis(); // Record the start time
}

void loop() {
  // Check if the specified delay has passed
  if (millis() - resetStartTime >= resetDelay) {
    // Set the pin high to trigger the reset
    digitalWrite(resetPin, HIGH);
    delay(100);  // Keep the pin high for a short duration
    // Set the pin back to low to complete the reset
    digitalWrite(resetPin, LOW);
  }

  // Your main program loop
}
