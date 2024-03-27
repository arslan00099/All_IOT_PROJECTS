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

String pass="";

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


void setup(){
  Serial.begin(115200);
  loadpref();
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey);
    pass+=customKey;

  }
  if(pass.indexOf("D")>-1){
   
    pass.remove(pass.length()-1);
     Serial.println(pass);
 preferences.begin("my-app", false);
  pass=preferences.putString("pass",pass);
  
  preferences.end();
  }
}


