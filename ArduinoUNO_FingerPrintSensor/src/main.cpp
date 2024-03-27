#include "Arduino.h"
String f_id="";
#include "biometric.h"
#include "Wire.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#include "keypadT.h"
unsigned long prevMillis=0;

void printLCD(String msg)
{
 lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(msg);
}
void setup()  
{
    lcd.init();                      // initialize the lcd 
  lcd.backlight();
  // Print a message to the LCD.
  pinMode(LockPIN,OUTPUT);
  digitalWrite(LockPIN,HIGH);
  Serial.begin(115200);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
    printLCD("Found FP sensor!");
  } else {
    Serial.println("Did not find  sensor :(");
     printLCD("Not Found FP sensor!");
    while (1) { delay(1); }
  }



}

void loop()                     // run over and over again
{
 
 enrolUser();
//delay(500);            //don't ned to run this at full speed.
getKey();
if(millis()-prevMillis>100)
{
  getFingerprintID();
  prevMillis=millis();
}
}
