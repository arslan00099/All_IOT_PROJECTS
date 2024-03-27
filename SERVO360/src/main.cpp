// #include <HardwareSerial.h>

// HardwareSerial Unit1(1); // Set up Serial2 on pins 16 (RX2) and 17 (TX2)

// void setup() {
//   Serial.begin(115200);
 
//   Unit1.begin(115200,SERIAL_8N1,17,16);
// }

// void loop() {
// while (Unit1.available())
// {
//   Unit1.write("HELLO WORLD");
// }

//   delay(1000); // Wait for 1 second
// }

#include <HardwareSerial.h>

HardwareSerial Unit2(1); // Set up Serial2 on pins 16 (RX2) and 17 (TX2)

void setup() {
Serial.begin(115200);
  Unit2.begin(115200,SERIAL_8N1,17,16);
}

void loop() {
while (Unit2.available())
{
  char Rxdchar=Unit2.read();
  Serial.println((String)Rxdchar);

}
}