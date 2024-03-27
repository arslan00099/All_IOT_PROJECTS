#include <MFRC522.h>
#include "httprequest.h"
String twoDimentionArray[7][7];
#define SS_PIN 21  /*Slave Select Pin*/
#define RST_PIN 22 /*Reset Pin for RC522*/
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
String lastScan="";

void showPage1Reason()
{
  Serial.println("Category 1 " + twoDimentionArray[0][0]);

  Serial.println("Reason: " + twoDimentionArray[1][0]);
  Serial.println("Reason: " + twoDimentionArray[2][0]);
  Serial.println("Reason: " + twoDimentionArray[3][0]);
  Serial.println("Reason: " + twoDimentionArray[4][0]);

  p1c1tb.setText(twoDimentionArray[0][0].c_str());
  p1tb1.setText(twoDimentionArray[1][0].c_str());
  p1tb2.setText(twoDimentionArray[2][0].c_str());
  p1tb3.setText(twoDimentionArray[3][0].c_str());
  p1tb4.setText(twoDimentionArray[4][0].c_str());
  p1tb5.setText(twoDimentionArray[5][0].c_str());
  p1tb6.setText(twoDimentionArray[6][0].c_str());
}

void showPage2Reason()
{
  Serial.println("Category 1 " + twoDimentionArray[0][1]);

  Serial.println("Reason: " + twoDimentionArray[1][1]);
  Serial.println("Reason: " + twoDimentionArray[2][1]);
  Serial.println("Reason: " + twoDimentionArray[3][1]);
  Serial.println("Reason: " + twoDimentionArray[4][1]);

  p1c1tb.setText(twoDimentionArray[0][1].c_str());
  p1tb1.setText(twoDimentionArray[1][1].c_str());
  p1tb2.setText(twoDimentionArray[2][1].c_str());
  p1tb3.setText(twoDimentionArray[3][1].c_str());
  p1tb4.setText(twoDimentionArray[4][1].c_str());
  p1tb5.setText(twoDimentionArray[5][1].c_str());
  p1tb6.setText(twoDimentionArray[6][1].c_str());
}

void showPage3Reason()
{
  Serial.println("Category 1 " + twoDimentionArray[0][2]);

  Serial.println("Reason: " + twoDimentionArray[1][2]);
  Serial.println("Reason: " + twoDimentionArray[2][2]);
  Serial.println("Reason: " + twoDimentionArray[3][2]);
  Serial.println("Reason: " + twoDimentionArray[4][2]);

  p1c1tb.setText(twoDimentionArray[0][2].c_str());
  p1tb1.setText(twoDimentionArray[1][2].c_str());
  p1tb2.setText(twoDimentionArray[2][2].c_str());
  p1tb3.setText(twoDimentionArray[3][2].c_str());
  p1tb4.setText(twoDimentionArray[4][2].c_str());
  p1tb5.setText(twoDimentionArray[5][2].c_str());
  p1tb6.setText(twoDimentionArray[6][2].c_str());
}

void showPage4Reason()
{
  Serial.println("Category 1 " + twoDimentionArray[0][3]);

  Serial.println("Reason: " + twoDimentionArray[1][3]);
  Serial.println("Reason: " + twoDimentionArray[2][3]);
  Serial.println("Reason: " + twoDimentionArray[3][3]);
  Serial.println("Reason: " + twoDimentionArray[4][3]);

  p1c1tb.setText(twoDimentionArray[0][3].c_str());
  p1tb1.setText(twoDimentionArray[1][3].c_str());
  p1tb2.setText(twoDimentionArray[2][3].c_str());
  p1tb3.setText(twoDimentionArray[3][3].c_str());
  p1tb4.setText(twoDimentionArray[4][3].c_str());
  p1tb5.setText(twoDimentionArray[5][3].c_str());
  p1tb6.setText(twoDimentionArray[6][3].c_str());
}

void showPage5Reason()
{
  Serial.println("Category 1 " + twoDimentionArray[0][4]);

  Serial.println("Reason: " + twoDimentionArray[1][4]);
  Serial.println("Reason: " + twoDimentionArray[2][4]);
  Serial.println("Reason: " + twoDimentionArray[3][4]);
  Serial.println("Reason: " + twoDimentionArray[4][4]);

  p1c1tb.setText(twoDimentionArray[0][4].c_str());
  p1tb1.setText(twoDimentionArray[1][4].c_str());
  p1tb2.setText(twoDimentionArray[2][4].c_str());
  p1tb3.setText(twoDimentionArray[3][4].c_str());
  p1tb4.setText(twoDimentionArray[4][4].c_str());
  p1tb5.setText(twoDimentionArray[5][4].c_str());
  p1tb6.setText(twoDimentionArray[6][4].c_str());
}

String readJSONFromFile(String filename)
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

void checkTagDataFromSpiff()
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
      if (usertype.indexOf("User") > -1)
      {
        userArray[0] = rfidtag;
        userArray[1] = usertype;
        userArray[2] = name;
        userFlag = true;
        String msg = "TAG ID: " + rfidtag + "\r\n" + "TAG TYPE: " + usertype + "\r\n" + "TAG NAME: " + name;
        showUserTag(msg);
      }
      else
      {
        vechileArray[0] = rfidtag;
        vechileArray[1] = usertype;
        vechileArray[2] = name;
        vechileFlag = true;
        String msg = "TAG ID: " + rfidtag + "\r\n" + "TAG TYPE: " + usertype + "\r\n" + "TAG NAME: " + name;
        showVehicleTag(msg);
      }
      Serial.println("TAG ID : " + (String)vechileArray[0]);
      Serial.println("USER TYPE : " + (String)usertype);
      Serial.println("NAME : " + (String)name);

      break;
    }
    else
    {
      // printrfidTag(rfidSt,"Not Found","Ct. Admin");
     // userFlag = true;
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
  if(lastScan != rfidSt){
    lastScan=rfidSt;
    Serial.println("############################ SAME TAG SCANNED ####################################3");
  checkTagDataFromSpiff();
  }

}

void readRFID()
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

  // mfrc522.PICC_HaltA();

  // mfrc522.PCD_StopCrypto1();
 // delay(100);
}

void getReasonFromSpiff()
{

  // ###  RESETING TWO DIMENTIONAL ARRAY ##############3
  for (int i = 0; i < 7; i++)
  {
    for (int j = 0; j < 7; j++)
    {
      twoDimentionArray[j][i] = "";
    }
  }
  // ##############   GETTING NEW VALUE FORM SPIFF ########
  String spiffData = readJSONFromFile("/reason.json");
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
  String previouscat = "Null";
  int cat_ind = -1;
  int res_ind = 0;
  for (int i = 0; i < doc.size(); i++)
  {
    // Serial.println("##################" + (String)i + "###############");
    String cat = doc[i]["category"];
    String reason = doc[i]["reason"];
    if (cat != previouscat)
    {
      res_ind = 0;
      cat_ind++;
      categorySize++;
      twoDimentionArray[res_ind][cat_ind] = cat;
      // Serial.print(twoDimentionArray[res_ind][cat_ind]);
      previouscat = cat;
      res_ind++;
      twoDimentionArray[res_ind][cat_ind] = reason;
      // Serial.print(twoDimentionArray[res_ind][cat_ind]);
    }
    else
    {
      res_ind++;
      twoDimentionArray[res_ind][cat_ind] = reason;
      // Serial.print(twoDimentionArray[res_ind][cat_ind]);
    }
  }
  Serial.println();
  for (int i = 0; i < 7; i++)
  {
    for (int j = 0; j < 7; j++)
    {
      Serial.print(twoDimentionArray[j][i] + "  ");
    }
    Serial.println();
  }
  Serial.println("Category 1 " + twoDimentionArray[0][0]);
  Serial.println("Category 2 " + twoDimentionArray[0][1]);
  Serial.println("Category 3 " + twoDimentionArray[0][2]);
}

void getMinMaxFromSpiff()
{

  // ##############   GETTING NEW VALUE FORM SPIFF ########
  String spiffData = readJSONFromFile("/minMaxKelo.json");
  DynamicJsonDocument doc(600);
  DeserializationError error = deserializeJson(doc, spiffData);

  // Test if parsing succeeds.
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  minkelometers = doc["minK"];
  maxkelometers = doc["maxK"];
  Serial.println("minkelometers : " + (String)minkelometers);
  Serial.println("maxkelometers : " + (String)maxkelometers);
}
