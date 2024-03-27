#include <MFRC522.h>
#include "httprequest.h"
String twoDimentionArray[6][31];  // 6 is row 30 colm
#define SS_PIN 21  /*Slave Select Pin*/
#define RST_PIN 34 /*Reset Pin for RC522*/
#define SIZE_BUFFER 18
#define MAX_SIZE_BLOCK 16
int arrayconsumed[6];


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
{  p1tb6.setText("");
  p1tb1.setText("");
  p1tb2.setText("");
  p1tb3.setText("");
  p1tb4.setText("");
  p1tb5.setText("");
  Serial.println("Category 1 " + twoDimentionArray[0][0]);

  Serial.println("Reason: " + twoDimentionArray[1][0]);
  Serial.println("Reason: " + twoDimentionArray[2][0]);
  Serial.println("Reason: " + twoDimentionArray[3][0]);
  Serial.println("Reason: " + twoDimentionArray[4][0]);
if(secondPage == 0){
  p1c1tb.setText(twoDimentionArray[0][0].c_str());
  p1tb1.setText(twoDimentionArray[1][0].c_str());
  p1tb2.setText(twoDimentionArray[2][0].c_str());
  p1tb3.setText(twoDimentionArray[3][0].c_str());
  p1tb4.setText(twoDimentionArray[4][0].c_str());
  p1tb5.setText(twoDimentionArray[5][0].c_str());
   if(twoDimentionArray[6][0] != nullptr){
  p1tb6.setText("Press # to move next");
  }
}

 
  


  if(secondPage == 1){
     p1tb6.setText("");
        Serial.println("secondpage is : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][0].c_str());
  p1tb1.setText(twoDimentionArray[6][0].c_str());
  p1tb2.setText(twoDimentionArray[7][0].c_str());
  p1tb3.setText(twoDimentionArray[8][0].c_str());
  p1tb4.setText(twoDimentionArray[9][0].c_str());
  p1tb5.setText(twoDimentionArray[10][0].c_str());
    if(twoDimentionArray[11][0] != nullptr){
   p1tb6.setText("# = next ; * = back");
  }
  }

   if(secondPage == 2){
    p1tb6.setText("");
        Serial.println("secondpage : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][0].c_str());
  p1tb1.setText(twoDimentionArray[11][0].c_str());
  p1tb2.setText(twoDimentionArray[12][0].c_str());
  p1tb3.setText(twoDimentionArray[13][0].c_str());
  p1tb4.setText(twoDimentionArray[14][0].c_str());
  p1tb5.setText(twoDimentionArray[15][0].c_str());
    if(twoDimentionArray[16][0] != nullptr){
   p1tb6.setText("# = next ; * = back");
  }
  }
     if(secondPage == 3){
      p1tb6.setText("");
        Serial.println("secondpage : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][0].c_str());
  p1tb1.setText(twoDimentionArray[16][0].c_str());
  p1tb2.setText(twoDimentionArray[17][0].c_str());
  p1tb3.setText(twoDimentionArray[18][0].c_str());
  p1tb4.setText(twoDimentionArray[19][0].c_str());
  p1tb5.setText(twoDimentionArray[20][0].c_str());
    if(twoDimentionArray[21][0] != nullptr){
     p1tb6.setText("# = next ; * = back");
  }
  }

      if(secondPage == 4){
      p1tb6.setText("");
        Serial.println("secondpage : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][0].c_str());
  p1tb1.setText(twoDimentionArray[21][0].c_str());
  p1tb2.setText(twoDimentionArray[22][0].c_str());
  p1tb3.setText(twoDimentionArray[23][0].c_str());
  p1tb4.setText(twoDimentionArray[24][0].c_str());
  p1tb5.setText(twoDimentionArray[25][0].c_str());
    if(twoDimentionArray[26][0] != nullptr){
   p1tb6.setText("# = next ; * = back");
  }
  }

     if(secondPage == 5){
      p1tb6.setText("");
        Serial.println("secondpage : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][0].c_str());
  p1tb1.setText(twoDimentionArray[26][0].c_str());
  p1tb2.setText(twoDimentionArray[27][0].c_str());
  p1tb3.setText(twoDimentionArray[28][0].c_str());
  p1tb4.setText(twoDimentionArray[29][0].c_str());
  p1tb5.setText(twoDimentionArray[30][0].c_str());
   p1tb6.setText("Press * to move back");
  }
   
  if(secondPage>=6){
    secondPage=6;
  }
  //p1tb6.setText(twoDimentionArray[6][0].c_str());
}

void showPage2Reason()
{
  p1tb6.setText("");
  p1tb1.setText("");
  p1tb2.setText("");
  p1tb3.setText("");
  p1tb4.setText("");
  p1tb5.setText("");
  Serial.println("Category 1 " + twoDimentionArray[0][1]);

  Serial.println("Reason: " + twoDimentionArray[1][1]);
  Serial.println("Reason: " + twoDimentionArray[2][1]);
  Serial.println("Reason: " + twoDimentionArray[3][1]);
  Serial.println("Reason: " + twoDimentionArray[4][1]);

 if(secondPage == 0){

  p1c1tb.setText(twoDimentionArray[0][1].c_str());
  p1tb1.setText(twoDimentionArray[1][1].c_str());
  p1tb2.setText(twoDimentionArray[2][1].c_str());
  p1tb3.setText(twoDimentionArray[3][1].c_str());
  p1tb4.setText(twoDimentionArray[4][1].c_str());
   p1tb5.setText(twoDimentionArray[5][1].c_str());
 // p1tb5.setText(twoDimentionArray[6][1].c_str());
   if((twoDimentionArray[6][1] != nullptr) && (twoDimentionArray[6][1] != "null")){
  p1tb6.setText("Press # to move next");
  }
}

 
  if(secondPage == 1){
     p1tb6.setText("");
        Serial.println("secondpage is : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][1].c_str());
  p1tb1.setText(twoDimentionArray[6][1].c_str());
  p1tb2.setText(twoDimentionArray[7][1].c_str());
  p1tb3.setText(twoDimentionArray[8][1].c_str());
  p1tb4.setText(twoDimentionArray[9][1].c_str());
  p1tb5.setText(twoDimentionArray[10][1].c_str());
    if(twoDimentionArray[11][1] != nullptr){
   p1tb6.setText("# = next ; * = back");
  }
  }

   if(secondPage == 2){
    p1tb6.setText("");
        Serial.println("secondpage : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][1].c_str());
  p1tb1.setText(twoDimentionArray[11][1].c_str());
  p1tb2.setText(twoDimentionArray[12][1].c_str());
  p1tb3.setText(twoDimentionArray[13][1].c_str());
  p1tb4.setText(twoDimentionArray[14][1].c_str());
  p1tb5.setText(twoDimentionArray[15][1].c_str());
    if(twoDimentionArray[16][1] != nullptr){
   p1tb6.setText("# = next ; * = back");
  }
  }
     if(secondPage == 3){
      p1tb6.setText("");
        Serial.println("secondpage : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][1].c_str());
  p1tb1.setText(twoDimentionArray[16][1].c_str());
  p1tb2.setText(twoDimentionArray[17][1].c_str());
  p1tb3.setText(twoDimentionArray[18][1].c_str());
  p1tb4.setText(twoDimentionArray[19][1].c_str());
  p1tb5.setText(twoDimentionArray[20][1].c_str());
    if(twoDimentionArray[21][1] != nullptr){
     p1tb6.setText("# = next ; * = back");
  }
  }

      if(secondPage == 4){
      p1tb6.setText("");
        Serial.println("secondpage : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][1].c_str());
  p1tb1.setText(twoDimentionArray[21][1].c_str());
  p1tb2.setText(twoDimentionArray[22][1].c_str());
  p1tb3.setText(twoDimentionArray[23][1].c_str());
  p1tb4.setText(twoDimentionArray[24][1].c_str());
  p1tb5.setText(twoDimentionArray[25][1].c_str());
    if(twoDimentionArray[26][1] != nullptr){
   p1tb6.setText("# = next ; * = back");
  }
  }

     if(secondPage == 5){
      p1tb6.setText("");
        Serial.println("secondpage : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][1].c_str());
  p1tb1.setText(twoDimentionArray[26][1].c_str());
  p1tb2.setText(twoDimentionArray[27][1].c_str());
  p1tb3.setText(twoDimentionArray[28][1].c_str());
  p1tb4.setText(twoDimentionArray[29][1].c_str());
  p1tb5.setText(twoDimentionArray[30][1].c_str());
   p1tb6.setText("Press * to move back");
  }
   
  if(secondPage>=6){
    secondPage=6;
  }
}

void showPage3Reason()
{  p1tb6.setText("");
  p1tb1.setText("");
  p1tb2.setText("");
  p1tb3.setText("");
  p1tb4.setText("");
  p1tb5.setText("");
  Serial.println("Category 1 " + twoDimentionArray[0][2]);

  Serial.println("Reason: " + twoDimentionArray[1][2]);
  Serial.println("Reason: " + twoDimentionArray[2][2]);
  Serial.println("Reason: " + twoDimentionArray[3][2]);
  Serial.println("Reason: " + twoDimentionArray[4][2]);

 if(secondPage == 0){
    p1tb6.setText("");
  p1c1tb.setText(twoDimentionArray[0][2].c_str());
  p1tb1.setText(twoDimentionArray[1][2].c_str());
  p1tb2.setText(twoDimentionArray[2][2].c_str());
  p1tb3.setText(twoDimentionArray[3][2].c_str());
  p1tb4.setText(twoDimentionArray[4][2].c_str());
  p1tb5.setText(twoDimentionArray[5][2].c_str());
   if(twoDimentionArray[6][2] != nullptr){
  p1tb6.setText("Press # to move next");
  }
}

 
  


  if(secondPage == 1){
     p1tb6.setText("");
        Serial.println("secondpage is : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][2].c_str());
  p1tb1.setText(twoDimentionArray[6][2].c_str());
  p1tb2.setText(twoDimentionArray[7][2].c_str());
  p1tb3.setText(twoDimentionArray[8][2].c_str());
  p1tb4.setText(twoDimentionArray[9][2].c_str());
  p1tb5.setText(twoDimentionArray[10][2].c_str());
    if(twoDimentionArray[11][2] != nullptr){
   p1tb6.setText("# = next ; * = back");
  }
  }

   if(secondPage == 2){
    p1tb6.setText("");
        Serial.println("secondpage : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][2].c_str());
  p1tb1.setText(twoDimentionArray[11][2].c_str());
  p1tb2.setText(twoDimentionArray[12][2].c_str());
  p1tb3.setText(twoDimentionArray[13][2].c_str());
  p1tb4.setText(twoDimentionArray[14][2].c_str());
  p1tb5.setText(twoDimentionArray[15][2].c_str());
    if(twoDimentionArray[16][2] != nullptr){
   p1tb6.setText("# = next ; * = back");
  }
  }
     if(secondPage == 3){
      p1tb6.setText("");
        Serial.println("secondpage : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][2].c_str());
  p1tb1.setText(twoDimentionArray[16][2].c_str());
  p1tb2.setText(twoDimentionArray[17][2].c_str());
  p1tb3.setText(twoDimentionArray[18][2].c_str());
  p1tb4.setText(twoDimentionArray[19][2].c_str());
  p1tb5.setText(twoDimentionArray[20][2].c_str());
    if(twoDimentionArray[21][2] != nullptr){
     p1tb6.setText("# = next ; * = back");
  }
  }

      if(secondPage == 4){
      p1tb6.setText("");
        Serial.println("secondpage : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][2].c_str());
  p1tb1.setText(twoDimentionArray[21][2].c_str());
  p1tb2.setText(twoDimentionArray[22][2].c_str());
  p1tb3.setText(twoDimentionArray[23][2].c_str());
  p1tb4.setText(twoDimentionArray[24][2].c_str());
  p1tb5.setText(twoDimentionArray[25][2].c_str());
    if(twoDimentionArray[26][2] != nullptr){
   p1tb6.setText("# = next ; * = back");
  }
  }

     if(secondPage == 5){
      p1tb6.setText("");
        Serial.println("secondpage : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][2].c_str());
  p1tb1.setText(twoDimentionArray[26][2].c_str());
  p1tb2.setText(twoDimentionArray[27][2].c_str());
  p1tb3.setText(twoDimentionArray[28][2].c_str());
  p1tb4.setText(twoDimentionArray[29][2].c_str());
  p1tb5.setText(twoDimentionArray[30][2].c_str());
   p1tb6.setText("Press * to move back");
  }
   
  if(secondPage>=6){
    secondPage=6;
  }
}

void showPage4Reason()
{  p1tb6.setText("");
  p1tb1.setText("");
  p1tb2.setText("");
  p1tb3.setText("");
  p1tb4.setText("");
  p1tb5.setText("");
  Serial.println("Category 1 " + twoDimentionArray[0][3]);

  Serial.println("Reason: " + twoDimentionArray[1][3]);
  Serial.println("Reason: " + twoDimentionArray[2][3]);
  Serial.println("Reason: " + twoDimentionArray[3][3]);
  Serial.println("Reason: " + twoDimentionArray[4][3]);

  if(secondPage == 0){
      p1tb6.setText("");
  p1c1tb.setText(twoDimentionArray[0][3].c_str());
  p1tb1.setText(twoDimentionArray[1][3].c_str());
  p1tb2.setText(twoDimentionArray[2][3].c_str());
  p1tb3.setText(twoDimentionArray[3][3].c_str());
  p1tb4.setText(twoDimentionArray[4][3].c_str());
  p1tb5.setText(twoDimentionArray[5][3].c_str());
   if(twoDimentionArray[6][3] != nullptr){
  p1tb6.setText("Press # to move next");
  }
}

 
  


  if(secondPage == 1){
     p1tb6.setText("");
        Serial.println("secondpage is : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][3].c_str());
  p1tb1.setText(twoDimentionArray[6][3].c_str());
  p1tb2.setText(twoDimentionArray[7][3].c_str());
  p1tb3.setText(twoDimentionArray[8][3].c_str());
  p1tb4.setText(twoDimentionArray[9][3].c_str());
  p1tb5.setText(twoDimentionArray[10][3].c_str());
    if(twoDimentionArray[11][3] != nullptr){
   p1tb6.setText("# = next ; * = back");
  }
  }

   if(secondPage == 2){
    p1tb6.setText("");
        Serial.println("secondpage : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][3].c_str());
  p1tb1.setText(twoDimentionArray[11][3].c_str());
  p1tb2.setText(twoDimentionArray[12][3].c_str());
  p1tb3.setText(twoDimentionArray[13][3].c_str());
  p1tb4.setText(twoDimentionArray[14][3].c_str());
  p1tb5.setText(twoDimentionArray[15][3].c_str());
    if(twoDimentionArray[16][3] != nullptr){
   p1tb6.setText("# = next ; * = back");
  }
  }
     if(secondPage == 3){
      p1tb6.setText("");
        Serial.println("secondpage : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][3].c_str());
  p1tb1.setText(twoDimentionArray[16][3].c_str());
  p1tb2.setText(twoDimentionArray[17][3].c_str());
  p1tb3.setText(twoDimentionArray[18][3].c_str());
  p1tb4.setText(twoDimentionArray[19][3].c_str());
  p1tb5.setText(twoDimentionArray[20][3].c_str());
    if(twoDimentionArray[21][3] != nullptr){
     p1tb6.setText("# = next ; * = back");
  }
  }

      if(secondPage == 4){
      p1tb6.setText("");
        Serial.println("secondpage : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][3].c_str());
  p1tb1.setText(twoDimentionArray[21][3].c_str());
  p1tb2.setText(twoDimentionArray[22][3].c_str());
  p1tb3.setText(twoDimentionArray[23][3].c_str());
  p1tb4.setText(twoDimentionArray[24][3].c_str());
  p1tb5.setText(twoDimentionArray[25][3].c_str());
    if(twoDimentionArray[26][3] != nullptr){
   p1tb6.setText("# = next ; * = back");
  }
  }

     if(secondPage == 5){
      p1tb6.setText("");
        Serial.println("secondpage : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][3].c_str());
  p1tb1.setText(twoDimentionArray[26][3].c_str());
  p1tb2.setText(twoDimentionArray[27][3].c_str());
  p1tb3.setText(twoDimentionArray[28][3].c_str());
  p1tb4.setText(twoDimentionArray[29][3].c_str());
  p1tb5.setText(twoDimentionArray[30][3].c_str());
   p1tb6.setText("Press * to move back");
  }
   
  if(secondPage>=6){
    secondPage=6;
  }
}

void showPage5Reason()
{  p1tb6.setText("");
  p1tb1.setText("");
  p1tb2.setText("");
  p1tb3.setText("");
  p1tb4.setText("");
  p1tb5.setText("");
  Serial.println("Category 1 " + twoDimentionArray[0][4]);

  Serial.println("Reason: " + twoDimentionArray[1][4]);
  Serial.println("Reason: " + twoDimentionArray[2][4]);
  Serial.println("Reason: " + twoDimentionArray[3][4]);
  Serial.println("Reason: " + twoDimentionArray[4][4]);

   if(secondPage == 0){
      p1tb6.setText("");
  p1c1tb.setText(twoDimentionArray[0][4].c_str());
  p1tb1.setText(twoDimentionArray[1][4].c_str());
  p1tb2.setText(twoDimentionArray[2][4].c_str());
  p1tb3.setText(twoDimentionArray[3][4].c_str());
  p1tb4.setText(twoDimentionArray[4][4].c_str());
  p1tb5.setText(twoDimentionArray[5][4].c_str());
   if(twoDimentionArray[6][4] != nullptr){
  p1tb6.setText("Press # to move next");
  }
}

 
  


  if(secondPage == 1){
     p1tb6.setText("");
        Serial.println("secondpage is : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][4].c_str());
  p1tb1.setText(twoDimentionArray[6][4].c_str());
  p1tb2.setText(twoDimentionArray[7][4].c_str());
  p1tb3.setText(twoDimentionArray[8][4].c_str());
  p1tb4.setText(twoDimentionArray[9][4].c_str());
  p1tb5.setText(twoDimentionArray[10][4].c_str());
    if(twoDimentionArray[11][4] != nullptr){
   p1tb6.setText("# = next ; * = back");
  }
  }

   if(secondPage == 2){
    p1tb6.setText("");
        Serial.println("secondpage : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][4].c_str());
  p1tb1.setText(twoDimentionArray[11][4].c_str());
  p1tb2.setText(twoDimentionArray[12][4].c_str());
  p1tb3.setText(twoDimentionArray[13][4].c_str());
  p1tb4.setText(twoDimentionArray[14][4].c_str());
  p1tb5.setText(twoDimentionArray[15][4].c_str());
    if(twoDimentionArray[16][4] != nullptr){
   p1tb6.setText("# = next ; * = back");
  }
  }
     if(secondPage == 3){
      p1tb6.setText("");
        Serial.println("secondpage : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][4].c_str());
  p1tb1.setText(twoDimentionArray[16][4].c_str());
  p1tb2.setText(twoDimentionArray[17][4].c_str());
  p1tb3.setText(twoDimentionArray[18][4].c_str());
  p1tb4.setText(twoDimentionArray[19][4].c_str());
  p1tb5.setText(twoDimentionArray[20][4].c_str());
    if(twoDimentionArray[21][4] != nullptr){
     p1tb6.setText("# = next ; * = back");
  }
  }

      if(secondPage == 4){
      p1tb6.setText("");
        Serial.println("secondpage : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][4].c_str());
  p1tb1.setText(twoDimentionArray[21][4].c_str());
  p1tb2.setText(twoDimentionArray[22][4].c_str());
  p1tb3.setText(twoDimentionArray[23][4].c_str());
  p1tb4.setText(twoDimentionArray[24][4].c_str());
  p1tb5.setText(twoDimentionArray[25][4].c_str());
    if(twoDimentionArray[26][4] != nullptr){
   p1tb6.setText("# = next ; * = back");
  }
  }

     if(secondPage == 5){
      p1tb6.setText("");
        Serial.println("secondpage : "+ (String)secondPage);
 p1c1tb.setText(twoDimentionArray[0][4].c_str());
  p1tb1.setText(twoDimentionArray[26][4].c_str());
  p1tb2.setText(twoDimentionArray[27][4].c_str());
  p1tb3.setText(twoDimentionArray[28][4].c_str());
  p1tb4.setText(twoDimentionArray[29][4].c_str());
  p1tb5.setText(twoDimentionArray[30][4].c_str());
   p1tb6.setText("Press * to move back");
  }
   
  if(secondPage>=6){
    secondPage=6;
  }
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
    
  }

  String spiffData = readJSONFromFile("/data.json");
  DynamicJsonDocument doc(5000);
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
        delay(2000);
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

    if(i == doc.size() - 1 ){
  Serial.println("############################# NEW CARD DETECTED ##################################");
  showUserTag("NEW RFID FOUND KINLDY REGISTER FIRST");
  showVehicleTag(rfidSt);
  delay(6000);
    
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
  //if(lastScan != rfidSt)
  {
    lastScan=rfidSt;
  
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
{categorySize = 0;

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
  DynamicJsonDocument doc(5000);
  DeserializationError error = deserializeJson(doc, spiffData);

  // Test if parsing succeeds.
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  Serial.println("REASON JSON SIZE : " + (String)doc.size());
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
  int repeat=-1;
  for (int i = 0; i < 7 ; i++)
  {repeat++;
    for (int j = 0; j < 31; j++)
    {
     // Serial.print(twoDimentionArray[j][i] + "  ");
       if (twoDimentionArray[j][i] != nullptr) {
      Serial.print(twoDimentionArray[j][i] + "  ");
      arrayconsumed[repeat]=j;
    } else {
      Serial.print("null  ");
    }
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
  DynamicJsonDocument doc(5000);
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
  relayTimeOut = doc["timeout"];
  Serial.println("minkelometers : " + (String)minkelometers);
  Serial.println("maxkelometers : " + (String)maxkelometers);
  Serial.println("RelayTimeOut : " + (String)relayTimeOut);
}
