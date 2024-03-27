#include <Arduino.h>

#include <Keypad.h>
#include <Preferences.h>
Preferences preferences;

int keyvalue;
int secondPage=0;
char customKey;
void showPage1Reason();

const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns
// define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

bool lockDoor = false;
String pass = "";
String cpass = "";
unsigned long startTime = 0;
int pos = 0;

byte rowPins[ROWS] = {13, 12, 14, 27}; /*Initialized ESP32 Pins for Rows*/
byte colPins[COLS] = {26, 25, 15, 5}; /*Initialized ESP32 Pins for Columns*/

// initialize an instance of class NewKeypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void loadpref()
{
  preferences.begin("my-app", false);
  pass = preferences.getString("pass");
  Serial.println(pass);
  preferences.end();
}

void loopkeypad()
{
  customKey = customKeypad.getKey();

     if (customKey){
prevMillisRef=millis();
Serial.println("Refresh Millis: "+(String)prevMillisRef);
     }
//        if(keyvalue == 48){
//       
// delay(3000);
// secondPage++;
// showPage1Reason();
// keyvalue=0;
//     }
     
  //     Serial.println(customKey);
  //      keyvalue= int(customKey);
  //      Serial.println("KEY VALUE : "+(String)keyvalue);
  //     cpass+=customKey;

  //   }
  //   if(cpass.indexOf("D")>-1){

  //      cpass.remove(cpass.length()-1);

  // Serial.println(cpass);

  //}
}