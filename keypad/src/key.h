#include <Arduino.h>

#include <Keypad.h>
#include <Preferences.h>
Preferences preferences;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

bool lockDoor = false;
String pass="";
String cpass="";
unsigned long startTime = 0;
int pos=0;
#define servoPin 22
Servo servo1;

byte rowPins[ROWS]      = {21, 19, 18, 5}; /*Initialized ESP32 Pins for Rows*/
byte colPins[COLS] = {12, 13, 14, 15};   /*Initialized ESP32 Pins for Columns*/

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void loadpref(){
  preferences.begin("my-app", false);
  pass=preferences.getString("pass");
  Serial.println(pass);
  preferences.end();
}


  
void loopkeypad(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey);
    cpass+=customKey;

  }
  if(cpass.indexOf("D")>-1){
   
     cpass.remove(cpass.length()-1);
//      Serial.println(pass);
//  preferences.begin("my-app", false);
//   preferences.putString("pass",cpass);
  
//   preferences.end();
 
Serial.println(cpass);
  if(cpass == pass){
    startTime=millis();
    servo1.write(180);
 lockDoor = true;
 Serial.println("SAME");
  cpass="";
  }
  else {
    Serial.println("NOT SAME");
     cpass="";
  }
 }
}