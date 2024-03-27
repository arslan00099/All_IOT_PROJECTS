#include <MFRC522.h>
#include "httprequest.h"

#define SS_PIN 16
#define RST_PIN 17
#define SIZE_BUFFER 18
#define MAX_SIZE_BLOCK 16

boolean userFlag = false;
boolean vechileFlag = false;
boolean menuFlag = true;
unsigned long flagcheckMillis = 0;
unsigned long checkepoch = 0;
unsigned long preMillis = 0;

byte nuidPICC[4] = {0, 0, 0, 0};

int item = 0;
// used in authentication
MFRC522::MIFARE_Key key;
// authentication return status code
MFRC522::StatusCode status;
// Defined pins to module RC522
MFRC522 mfrc522(SS_PIN, RST_PIN);
String userArray[3];
String vechileArray[4];
String info[2];
int reasonMenu = 1;

String readJSONFromFile(const char *filename)
{
  File file = SPIFFS.open(filename);
  if (!file)
  {
    Serial.println("Failed to open file");
    return "";
  }
  String data = file.readString();
  Serial.println(data);
  file.close();
  return data;
}

void checkFromSpiff()
{

  if ((userFlag == false) && (vechileFlag == false))
  {
    flagcheckMillis = millis();
    Serial.println("MILLIS set to ZERO");
  }

  String spiffData = readJSONFromFile("/data.json");
  DynamicJsonDocument doc(2500);
  DeserializationError error = deserializeJson(doc, spiffData);

  // Test if parsing succeeds.
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  Serial.println("SIZE" + (String)doc.size());

  for (int i = 0; i < doc.size(); i++)
  {
    Serial.println("##################" + (String)i + "###############");
    String rfidtag = doc[i]["rfidnum"];
    String usertype = doc[i]["usertyp"];
    String name = doc[i]["name"];
    String kmeters = doc[i]["kmeters"];
    if (rfidSt == rfidtag)
    {
      // printrfidTag(rfidtag,usertype,name);
      if (usertype.indexOf("user") > -1)
      {
        userArray[0] = rfidtag;
        userArray[1] = usertype;
        userArray[2] = name;
        userFlag = true;
      }
      else
      {
        vechileArray[0] = rfidtag;
        vechileArray[1] = usertype;
        vechileArray[2] = name;
        vechileFlag = true;
      }
      Serial.println("TAG ID : " + (String)vechileArray[0]);
      Serial.println("USER TYPE : " + (String)usertype);
      Serial.println("NAME : " + (String)name);

      break;
    }
    else
    {
      // printrfidTag(rfidSt,"Not Found","Ct. Admin");
      userFlag = true;
    }
  }
}

void printDec(byte *buffer, byte bufferSize)
{
  rfidSt = "";
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
    rfidSt += buffer[i];
  }
  Serial.println("\r\n########## TAG Found ######");
  Serial.println("RFID Tag Value: " + rfidSt);
  checkFromSpiff();
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
