#include <Arduino.h>
#include "next.h"
#include "key.h"
#include "rfidscanner.h"
#include "myntp.h"
#include "flow.h"
//#include "hc12.h"

#define pump_RELAY 4
String reasonArray[5];
int counter = 0;
int inputKelo;
float value;
char buffer[10];
;
int lastKeloUsed;

const char *ssid = "RIGJAZZ";
const char *password = "Kashmiri786";

void setupWifi()
{
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void resetSPIFFS()
{
  SPIFFS.end();
  SPIFFS.format();
  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }
}

void handlingMenus()
{
  if (counter < categorySize)
  {
    if (customKey)
    {

      keyvalue = int(customKey);
      Serial.println("KEY VALUE : " + (String)keyvalue);
      if (keyvalue == 68) // D clicked
      {
        p1tb1.setFont(1);
        p1tb2.setFont(1);
        p1tb3.setFont(1);
        p1tb4.setFont(1);
        Serial.println(cpass);
        if (cpass.indexOf("1") > -1)
        {
          reasonArray[counter] = twoDimentionArray[1][counter];
        }
        if (cpass.indexOf("2") > -1)
        {
          reasonArray[counter] = twoDimentionArray[2][counter];
        }
        if (cpass.indexOf("3") > -1)
        {
          reasonArray[counter] = twoDimentionArray[3][counter];
        }
        if (cpass.indexOf("4") > -1)
        {
          reasonArray[counter] = twoDimentionArray[4][counter];
        }
        if (cpass.indexOf("5") > -1)
        {
          reasonArray[counter] = twoDimentionArray[5][counter];
        }
        if (cpass.indexOf("6") > -1)
        {
          reasonArray[counter] = twoDimentionArray[6][counter];
        }
        Serial.println("REASON " + (String)counter + 1 + " SELECTED :" + (String)reasonArray[counter]);
        keyvalue = 0;
        counter++;
        Serial.println((String) + counter);
        customKey = 'A';
        if (counter == categorySize)
        {
          Serial2.print("page page2");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);
          customKey = 'A';
          counter = 5;
        }

        else if (counter == 1)
        {
          showPage2Reason();
        }
        else if (counter == 2)
        {
          showPage3Reason();
        }
        else if (counter == 3)
        {
          showPage4Reason();
        }
        else if (counter == 4)
        {
          showPage5Reason();
        }
      }
      //END OF D selected IF 

      if (keyvalue == 42)  // for Going Back
      {
        Serial.println("Counter --");
        counter--;
        if (counter == 0)
        {
          showPage1Reason();
        }
        else if (counter == 1)
        {
          showPage2Reason();
        }
        else if (counter == 2)
        {
          showPage3Reason();
        }
        else if (counter == 3)
        {
          showPage4Reason();
        }
        else if (counter == 4)
        {
          showPage5Reason();
        }
      }

      else
      {
        cpass = customKey;
        // Serial.println("KEYVAKE : "+(String)keyvalue);
        if (keyvalue == 49)
        {
          p1tb1.setFont(3);
          p1tb2.setFont(1);
          p1tb3.setFont(1);
          p1tb4.setFont(1);
          p1tb5.setFont(1);
          p1tb6.setFont(1);
        }
        else if (keyvalue == 50)
        {
          p1tb1.setFont(1);
          p1tb2.setFont(3);
          p1tb3.setFont(1);
          p1tb4.setFont(1);
          p1tb5.setFont(1);
          p1tb6.setFont(1);
        }
        else if (keyvalue == 51)
        {
          p1tb1.setFont(1);
          p1tb2.setFont(1);
          p1tb3.setFont(3);
          p1tb4.setFont(1);
          p1tb5.setFont(1);
          p1tb6.setFont(1);
        }
        else if (keyvalue == 52)
        {
          p1tb1.setFont(1);
          p1tb2.setFont(1);
          p1tb3.setFont(1);
          p1tb4.setFont(3);
          p1tb5.setFont(1);
          p1tb6.setFont(1);
        }
        else if (keyvalue == 53)
        {
          p1tb1.setFont(1);
          p1tb2.setFont(1);
          p1tb3.setFont(1);
          p1tb4.setFont(1);
          p1tb5.setFont(3);
          p1tb6.setFont(1);
        }
        else if (keyvalue == 54)
        {
          p1tb1.setFont(1);
          p1tb2.setFont(1);
          p1tb3.setFont(1);
          p1tb4.setFont(1);
          p1tb5.setFont(1);
          p1tb6.setFont(3);
        }
      }
    }
  }

  if (counter > 4)
  {
    keyvalue = int(customKey);
    // Serial.println((String)keyvalue);
    if (customKey)
    {
      if (keyvalue == 67)
      {
        cpass.remove(cpass.length() - 1);
        p2tb1.setText(cpass.c_str());
      }
      else if (keyvalue == 65)
      {
        String empty = "";
        cpass = "";
        p2tb1.setText(empty.c_str());
      }
      else
      {
        Serial.println(keyvalue);
        keyvalue = int(customKey);
        cpass += customKey;
        p2tb1.setText(cpass.c_str());
      }
    }
    
    if (cpass.indexOf("D") > -1)
    {

      cpass.remove(cpass.length() - 1);
      p2tb1.setText(cpass.c_str());
      inputKelo = cpass.toInt();
      if ((inputKelo > minkelometers) && (inputKelo < maxkelometers) && ((inputKelo > lastKeloUsed)))
      {
        digitalWrite(pump_RELAY, HIGH);
        cpass += "  Good";
        flowFlag = true;
        postMillis = millis();
        p2tb1.setText(cpass.c_str());
        Serial.println("RELAY ON");
      }

      if (inputKelo < lastKeloUsed)
      {
        cpass += "  LESS then Last Used";
        p2tb1.setText(cpass.c_str());
        cpass = "";
      }

      if (inputKelo < minkelometers)
      {
        cpass += "  LOW";
        p2tb1.setText(cpass.c_str());
        cpass = "";
      }

      if (inputKelo > maxkelometers)
      {
        cpass += "  HIGH";
        p2tb1.setText(cpass.c_str());
        customKey = 'A';
        cpass = "";
      }

      Serial.println(cpass);
    }
  }
}

void listMySpiff()
{
  Serial.println("######################  LISTING SPIFF #######################");
  File root = SPIFFS.open("/rememberKelo.json");

  File file = root.openNextFile();

  while (file)
  {

    Serial.print("FILE: ");
    Serial.println(file.name());

    file = root.openNextFile();
  }
  Serial.println("######################  END SPIFF #######################");
}

void uploadSpiffFile()
{
  File root = SPIFFS.open("/uploadData");

  File file = root.openNextFile();

  while (file)
  {

    Serial.print("FILE: ");
    Serial.println(file.name());
    String name = "/uploadData/" + (String)file.name();
    String serverData = readJSONFromFile(name);
    // Serial.println(serverData);
    postData(serverData);
    SPIFFS.remove(name);
    file = root.openNextFile();
  }
}

void resetAll()
{

  digitalWrite(pump_RELAY, LOW);
  delay(5000);
  postFlag = false;
  counter = 0;
  inputKelo = 0;
  finalLiters = 0;
  value = 0;

  for (int i = 0; i < 10; i++)
  {
    char buffer[i] = "0";
  }

  Serial2.print("page page0");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  resetFlag = false;
}


void rememberKelomSpiff(String rfidnum,int kilom){
  const char* jsonFilePath = "/rememberKelo.json";
   File jsonFile = SPIFFS.open(jsonFilePath, "r");
  if (jsonFile) {
    size_t fileSize = jsonFile.size();
    std::unique_ptr<char[]> jsonBuffer(new char[fileSize]);
    jsonFile.readBytes(jsonBuffer.get(), fileSize);

    // Parse the JSON data
    DynamicJsonDocument jsonData(1024);
    DeserializationError error = deserializeJson(jsonData, jsonBuffer.get());
    if (error) {
      Serial.println("Failed to parse JSON file");
         Serial.println(error.f_str());
      return;
    }

    // Close the file
    jsonFile.close();

    // Define the RFID number and kilom value
    // const char* rfidnum = "your_rfid_number";
    // int kilom = 100;

    // Check if the RFID number exists in the JSON data
    if (jsonData.containsKey(rfidnum)) {
      // Update the kilom value
      jsonData[rfidnum]["kilom"] = kilom;
    } else {
      // Add the RFID number with the kilom value
      jsonData[rfidnum]["kilom"] = kilom;
    }

    // Save the updated JSON data to SPIFFS
    jsonFile = SPIFFS.open(jsonFilePath, "w");
    if (jsonFile) {
      serializeJson(jsonData, jsonFile);
      jsonFile.close();
      Serial.println("JSON data saved successfully");
    } else {
      Serial.println("Failed to open JSON file for writing");
    }
  } else {
    Serial.println("Failed to open JSON file");
  }
}

void setup()
{
  Serial.begin(115200);
  setupNExtion();
  // resetSPIFFS();
  pinMode(pump_RELAY, OUTPUT);
  digitalWrite(pump_RELAY, LOW);

  SPI.begin();        /*SPI communication initialized*/
  mfrc522.PCD_Init(); /*RFID sensor initialized*/

  if (!SPIFFS.begin(true))
  {
    Serial.println("Failed to mount file system");
    return;
  }
  setupFlowSensor();
  getReasonFromSpiff();
  setupWifi();
  getMinMaxFromSpiff();
  getEpoch();

  while (epochTime < 1)
  {
    getntp();
  }
  Serial.println("EPOUCH : " + (String)rtc.getEpoch());

  Serial.println("Approach your reader card...");
  Serial.println();
  Serial2.print("page page0");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);



    // Create a JSON object
  DynamicJsonDocument data(128);
  data["123456"] = 30;


  // Open the file in write mode
  File file = SPIFFS.open("/rememberKelo.json", FILE_WRITE);
  if (!file) {
    Serial.println("Error opening file");
    return;
  }

  // Serialize the JSON object to the file
  if (serializeJson(data, file) == 0) {
    Serial.println("Failed to write to file");
  } else {
    Serial.println("Data written to data.json");
  }

  // Close the file
  file.close();

  listMySpiff();
  uploadSpiffFile();
 // setupHC();
}

void loop()
{

  // String data = checkHCValue();
  // if (data.length() > 1)
  // {
  //   Serial.println("SCANN : "+(String)data);
  //   data.trim();
  //   rfidSt = data;
  //   checkTagDataFromSpiff();
  // }

  loopNextion();

  readRFID();
  loopkeypad();
  handlingMenus();
  if (millis() - checkepoch > 20000)
  {
    getEpoch();
     mfrc522.PCD_Init(); //RFID sensor initialized
    checkepoch = millis();
  }

  if ((userFlag == true) || (vechileFlag == true))
  {
    if ((millis() - flagcheckMillis < 15000) && (userFlag == true) && (vechileFlag == true))
    {
      Serial.println("BOTH ARE ASSIGNED");

      Serial2.print("page page1");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      showPage1Reason();

      Serial.println("MAIN MENU WILL BE OPENED");

      for (int i = 0; i < vechileRfidArray->length(); i++)
      {
        Serial.println(i);
        Serial.println("VECHILE TAG : " + (String)vechileArray[0]);
        Serial.println("RFID TAG : " + (String)vechileRfidArray[i]);

        if (vechileArray[0] == vechileRfidArray[i])
        {
          Serial.print("############################################## RFID FOUND ###################################");
          Serial.println("LAST KELO USED BY THIS ID  : "+(String)keloArray[i]);

          String lastval = keloArray[i];
          // Serial.println("LATVALIE : "+(String)lastval);
          lastKeloUsed = lastval.toInt();
          exit;
        }
      }

      userFlag = false;
      vechileFlag = false;
    }
    else
    {
      if (millis() - flagcheckMillis > 15000)
      {
        Serial.println("FLAG RESET");
        Serial.println("SCAN AGAIN");
        showUserTag("USER TAG");
        showVehicleTag("VECHICLE TAG");
        userFlag = false;
        vechileFlag = false;
      }
    }
  }
  else
  {
    //  Serial.println("ENTER SECOND TAG");
  }
  // ##########  CONDITION FOR FLOW METER
  if (flowFlag == true)
  {
    value = CheckFlow(calibrationFactor);
    dtostrf(value, 5, 2, buffer); // Convert the float to a string with 5 digits, 2 decimal places, and store it in the buffer
    Serial.println(buffer);
    String a = buffer;
    p2tb1.setText(a.c_str());
  }
  else
  {
    finalLiters = 0;
  }

  if (postFlag == true)
  {
    String postStr = "{\"staid\":\"" + (String)station_ID + "\",\"urfid\":\"" + (String)userArray[0] + "\",\"vrfid\":\"" + (String)vechileArray[0] + "\",\"rone\":\"" + (String)reasonArray[0] + "\",\"rtwo\":\"" + (String)reasonArray[1] + "\",\"rthree\":\"" + (String)reasonArray[2] + "\",\"rfour\":\"" + (String)reasonArray[3] + "\",\"rfive\":\"" + (String)reasonArray[4] + "\",\"km\":\"" + (String)inputKelo + "\",\"fuel\":\"" + (String)finalLiters + "\",\"epoch\":\"" + (String)rtc.getEpoch() + "\"}";
Serial.println(postStr);
//Saving last kelometers in spiff;
rememberKelomSpiff(vechileArray[0],inputKelo);

    if (WiFi.status() != WL_CONNECTED)
    {
      String time = (String)rtc.getEpoch();
      String fileName = "/uploadData/" + time + ".json";
      writeJsonToSpiff(fileName.c_str(), postStr.c_str());
    }
    else
    {
      postData(postStr);
      Serial.println("POSTING");
      String a = "Sucessfully Posted";
      p2tb1.setText(a.c_str());
      resetFlag = true;
    }
  }

  if (resetFlag == true)
  {
    resetAll();
  }

 
}