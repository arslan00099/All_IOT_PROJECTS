

#include <Keypad.h>
String lockPSWD="12345";
const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns

String pswd="";
// Map the buttons to an array for the Keymap instance
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte colPins[ROWS] = {7, 6, 5, 4}; // Pins used for the rows of the keypad
byte rowPins[COLS] = {11, 10, 9, 8}; // Pins used for the columns of the keypad

// Initialise the Keypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

boolean getKeyID()
{
char button = customKeypad.getKey();
if(button)
{
if(button=='#')
{
  Serial.println("Save ID");
  printLCD("Save ID");
return true;
}
  else  {
    f_id+=button;
    Serial.println(button);
  Serial.println("Key: "+(f_id));
  printLCD("Key: "+(f_id));
  }
return false;
}
}

void getKey() {

  // Read the pushed button
  char button = customKeypad.getKey();

if(button=='A')
{
  pswd="";
Serial.println("Enroll New User");
printLCD("Enroll New User");
Serial.println("Enter Admin PSWD");
enrolUser();

}

  if (button) {
    if(button!='#')
    pswd+=button;
    Serial.println(button);
  Serial.println("Password: "+(pswd));


  }
if(button=='#')
{
  Serial.println("UNLOCK");
if(pswd.compareTo(lockPSWD)==0)
{
Serial.println("PSWD Matched");
digitalWrite(LockPIN,LOW);
    unsigned long prevMilliHold=millis();
    while (millis()-prevMilliHold<2000)
    {
        /* code */
        
    }
    digitalWrite(LockPIN,HIGH);
pswd="";
}
else
{
  Serial.println("PSWD not Matched");
  pswd="";
}
}


}
