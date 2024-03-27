

#include <Arduino.h>
String rfidSt="";

#include <MFRC522.h>

#define SS_PIN 21  /*Slave Select Pin*/
#define RST_PIN 34 /*Reset Pin for RC522*/
#define SIZE_BUFFER 18
#define MAX_SIZE_BLOCK 16

boolean userFlag = false;
boolean vechileFlag = false;
boolean menuFlag = true;
unsigned long flagcheckMillis = 0;
unsigned long checkepoch = 0;
unsigned long preMillis = 0;
String categoryArray[4];

byte nuidPICC[4] = {0, 0, 0, 0};

int item = 0;
int categorySize = 0;
String lastscanid;
// used in authentication
MFRC522::MIFARE_Key key;
// authentication return status code
MFRC522::StatusCode status;
// Defined pins to module RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); /*Create MFRC522 initialized*/
String userArray[3];
String vechileArray[3];
String info[2];
int reasonMenu = 1;



void printDec(byte *buffer, byte bufferSize)
{
   lastscanid="";
  rfidSt = "";
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
    rfidSt += buffer[i];
  }
  Serial.println("\r\n########## TAG Found ######");
  Serial.println("RFID Tag Value: " + rfidSt);
  lastscanid=rfidSt;

}

void readRFID(void)
{

  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }

  if (!mfrc522.PICC_IsNewCardPresent())
    return;

  if (!mfrc522.PICC_ReadCardSerial())
    return;

  for (byte i = 0; i < 4; i++)
  {
    nuidPICC[i] = mfrc522.uid.uidByte[i];
  }
  Serial.print(F("RFID In dec: "));
  printDec(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();

  mfrc522.PICC_HaltA();

  mfrc522.PCD_StopCrypto1();
  delay(100);
}
