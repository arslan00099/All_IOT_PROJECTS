#include <Arduino.h>
#include <EEPROM.h>

const int pulsePin = 15;  // Change this to your desired input pin
volatile int pulseCount = 0;

void IRAM_ATTR pulseInterrupt() {
  pulseCount++;
}

void setup() {
  Serial.begin(115200);
  Serial.println("HELLO");
  pinMode(pulsePin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pulsePin), pulseInterrupt, FALLING);

  EEPROM.begin(sizeof(int));
  int savedCount;
  EEPROM.get(0, savedCount);
  pulseCount = savedCount;
}

void loop() {
  // Your main loop code goes here
  // This is just an example to show how the pulse count is updated

  static unsigned long prevMillis = 0;
  unsigned long currentMillis = millis();

  if (currentMillis - prevMillis >= 1000) {
    prevMillis = currentMillis;
    Serial.println("Pulse Count: " + String(pulseCount));
    
    // Save pulse count to EEPROM
    EEPROM.put(0, pulseCount);
    EEPROM.commit();
  }
}
