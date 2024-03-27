#include <Arduino.h>
float finalLiters;
float value;
unsigned long prevMillisRef = 0;
#include "next.h"
#include "key.h"
#include "rfidscanner.h"
#include "myntp.h"
#include "flow.h"
#include "rtc.h"
#include "hc12.h"
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
#include "soc/rtc_wdt.h"

unsigned long prevMillRTC = 0;
TaskHandle_t Task1;
TaskHandle_t Task2;
#define pump_RELAY 2
String reasonArray[5];
int counter = 0;
int inputKelo;
unsigned long showTimeMillis=0;
char buffer[10];
int lastKeloUsed;

// const char *ssid = "RIGJAZZ";
// const char *password = "Kashmiri786";
const char *ssid = "Islo AI";
const char *password = "gamican3506";

void getlastkm()
{

  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount SPIFFS to get lastKelo");
    return;
  }

  // Open the JSON file
  File file = SPIFFS.open("/rememberKelo.json", "r");
  if (!file)
  {
    Serial.println("Failed to open file");
    return;
  }

  // Read the contents of the file into a buffer
  StaticJsonDocument<256> jsonDoc; // Adjust the capacity as per your JSON size
  DeserializationError error = deserializeJson(jsonDoc, file);
  if (error)
  {
    Serial.print("Failed to parse JSON file: ");
    Serial.println(error.c_str());
    return;
  }

  // Close the file
  file.close();

  // Extract RFID numbers and kilom values into arrays
  const size_t capacity = jsonDoc.size();
  String rfidArray[capacity];
  String keloArray[capacity];
  size_t index = 0;

  // Iterate through the JSON object
  for (const auto &entry : jsonDoc.as<JsonObject>())
  {
    const String &rfid = entry.key().c_str();
    String kilom = entry.value()["kilom"];

    // Store RFID number and kilom value in respective arrays
    rfidArray[index] = rfid;
    keloArray[index] = kilom;
    if (vechileArray[0] == rfidArray[index])
    {
      Serial.print("############################################## RFID FOUND ###################################");
      Serial.println("LAST KELO USED BY THIS ID  : " + (String)keloArray[index]);
      Serial.print("RFID: ");
      Serial.print(rfidArray[index]);
      Serial.print(", Kilom: ");
      Serial.println(keloArray[index]);
      String lastval = keloArray[index];
      // Serial.println("LATVALIE : "+(String)lastval);
      lastKeloUsed = lastval.toInt();
      exit;
    }

    index++;
  }

  // Print the arrays
  for (size_t i = 0; i < capacity; i++)
  {
    Serial.print("RFID: ");
    Serial.print(rfidArray[i]);
    Serial.print(", Kilom: ");
    Serial.println(keloArray[i]);
  }
}

void readHC12()
{
  // ############# SCAN RFID HC12################
  String data = checkHCValue();
  if (data.length() > 1)
  {
    Serial.println("SCANN : " + (String)data);
    data.trim();
    rfidSt = data;
    checkTagDataFromSpiff();
  }
  // ############# SCAN RFID HC12################
}
void setupWifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  unsigned long prevMillisWifi = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - prevMillisWifi < 20000)
  { 
  timeinfo.setText("Connecting Wifi");
    readHC12();
    delay(500);
    Serial.print(".");
    TIMERG1.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
    TIMERG1.wdt_feed = 1;
    TIMERG1.wdt_wprotect = 0;
    TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
    TIMERG0.wdt_feed = 1;
    TIMERG0.wdt_wprotect = 0;
  }
  if (WiFi.isConnected())
  {
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.print("WiFi not connected");
  }
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
  else {
      Serial.println("SUCESSFULLY RESET");
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
          p1tb1.Set_font_color_pco(0);
          p1tb2.Set_font_color_pco(0);
          p1tb3.Set_font_color_pco(0);
          p1tb4.Set_font_color_pco(0);
          p1tb5.Set_font_color_pco(0);
         
        Serial.println(cpass);
        if ((cpass.indexOf("1") > -1) && (secondPage == 0))
        {
          reasonArray[counter] = twoDimentionArray[1][counter];
        }
        if ((cpass.indexOf("2") > -1) && (secondPage == 0))
        {
          reasonArray[counter] = twoDimentionArray[2][counter];
        }
        if ((cpass.indexOf("3") > -1) && (secondPage == 0))
        {
          reasonArray[counter] = twoDimentionArray[3][counter];
        }
        if ((cpass.indexOf("4") > -1) && (secondPage == 0))
        {
          reasonArray[counter] = twoDimentionArray[4][counter];
        }
        if ((cpass.indexOf("5") > -1) && (secondPage == 0))
        {
          reasonArray[counter] = twoDimentionArray[5][counter];
        }
        // if ((cpass.indexOf("6") > -1) && (secondPage == 0))
        // {
        //   reasonArray[counter] = twoDimentionArray[6][counter];
        // }
          if ((cpass.indexOf("1") > -1) && (secondPage == 1))
        {
          reasonArray[counter] = twoDimentionArray[6][counter];
        }
        if ((cpass.indexOf("2") > -1) && (secondPage == 1))
        {
          reasonArray[counter] = twoDimentionArray[7][counter];
        }
        if ((cpass.indexOf("3") > -1) && (secondPage == 1))
        {
          reasonArray[counter] = twoDimentionArray[8][counter];
        }
        if ((cpass.indexOf("4") > -1) && (secondPage == 1))
        {
          reasonArray[counter] = twoDimentionArray[9][counter];
        }
        if ((cpass.indexOf("5") > -1) && (secondPage == 1))
        {
          reasonArray[counter] = twoDimentionArray[10][counter];
        }
        //#################3
          if ((cpass.indexOf("1") > -1) && (secondPage == 2))
        {
          reasonArray[counter] = twoDimentionArray[11][counter];
        }
        if ((cpass.indexOf("2") > -1) && (secondPage == 2))
        {
          reasonArray[counter] = twoDimentionArray[12][counter];
        }
        if ((cpass.indexOf("3") > -1) && (secondPage == 2))
        {
          reasonArray[counter] = twoDimentionArray[13][counter];
        }
        if ((cpass.indexOf("4") > -1) && (secondPage == 2))
        {
          reasonArray[counter] = twoDimentionArray[14][counter];
        }
        if ((cpass.indexOf("5") > -1) && (secondPage == 2))
        {
          reasonArray[counter] = twoDimentionArray[15][counter];
        }
        //########
          if ((cpass.indexOf("1") > -1) && (secondPage == 3))
        {
          reasonArray[counter] = twoDimentionArray[16][counter];
        }
        if ((cpass.indexOf("2") > -1) && (secondPage == 3))
        {
          reasonArray[counter] = twoDimentionArray[17][counter];
        }
        if ((cpass.indexOf("3") > -1) && (secondPage == 3))
        {
          reasonArray[counter] = twoDimentionArray[18][counter];
        }
        if ((cpass.indexOf("4") > -1) && (secondPage == 3))
        {
          reasonArray[counter] = twoDimentionArray[19][counter];
        }
        if ((cpass.indexOf("5") > -1) && (secondPage == 3))
        {
          reasonArray[counter] = twoDimentionArray[20][counter];
        }
        //######
          if ((cpass.indexOf("1") > -1) && (secondPage == 4))
        {
          reasonArray[counter] = twoDimentionArray[21][counter];
        }
        if ((cpass.indexOf("2") > -1) && (secondPage == 4))
        {
          reasonArray[counter] = twoDimentionArray[22][counter];
        }
        if ((cpass.indexOf("3") > -1) && (secondPage == 4))
        {
          reasonArray[counter] = twoDimentionArray[23][counter];
        }
        if ((cpass.indexOf("4") > -1) && (secondPage == 4))
        {
          reasonArray[counter] = twoDimentionArray[24][counter];
        }
        if ((cpass.indexOf("5") > -1) && (secondPage == 4))
        {
          reasonArray[counter] = twoDimentionArray[25][counter];
        }
        //###########
          if ((cpass.indexOf("1") > -1) && (secondPage == 5))
        {
          reasonArray[counter] = twoDimentionArray[26][counter];
        }
        if ((cpass.indexOf("2") > -1) && (secondPage == 5))
        {
          reasonArray[counter] = twoDimentionArray[27][counter];
        }
        if ((cpass.indexOf("3") > -1) && (secondPage == 5))
        {
          reasonArray[counter] = twoDimentionArray[28][counter];
        }
        if ((cpass.indexOf("4") > -1) && (secondPage == 5))
        {
          reasonArray[counter] = twoDimentionArray[29][counter];
        }
        if ((cpass.indexOf("5") > -1) && (secondPage == 5))
        {
          reasonArray[counter] = twoDimentionArray[30][counter];
        }
        //############


        //###################################################################
        //  if ((cpass.indexOf("1") > -1) && (secondPage == 1))
        // {
        //   reasonArray[counter] = twoDimentionArray[6][counter];
        // }
        // if ((cpass.indexOf("2") > -1) && (secondPage == 1))
        // {
        //   reasonArray[counter] = twoDimentionArray[7][counter];
        // }
        // if ((cpass.indexOf("3") > -1) && (secondPage == 1))
        // {
        //   reasonArray[counter] = twoDimentionArray[8][counter];
        // }
        // if ((cpass.indexOf("4") > -1) && (secondPage == 1))
        // {
        //   reasonArray[counter] = twoDimentionArray[9][counter];
        // }
        // if ((cpass.indexOf("5") > -1) && (secondPage == 1))
        // {
        //   reasonArray[counter] = twoDimentionArray[10][counter];
        // }
        // if ((cpass.indexOf("6") > -1) && (secondPage == 1))
        // {
        //   reasonArray[counter] = twoDimentionArray[11][counter];
        // }
int c=counter+1;
Serial.println();
Serial.println("###########################################################################");
        Serial.println("REASON " + (String)c + " SELECTED : " + (String)reasonArray[counter]);
        Serial.println("COUNTER : "+(String)counter);
        Serial.println("CATEGORY SIZE : "+(String)categorySize);
Serial.println("###########################################################################");
        secondPage=0;
        keyvalue = 0;
        counter++;
        Serial.println((String) + counter);
        customKey = 'A';
        if (counter == categorySize)
        {
          Serial.println("MOVING TO PAGE 2");
          Serial2.print("page page2");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);
          customKey = 'A';
          counter = 5;
          finalLiters=0;
           String msg="KM/H: "+ vechileArray[2];
          carinfo.setText(msg.c_str());
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
      // END OF D selected IF

      if (keyvalue == 66) // for Going Back
      {secondPage=0;
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
        if (keyvalue == 49)  //49 is for 1
        {
          Serial.println("Zoom 1");

          p1tb1.Set_font_color_pco(63488);
          p1tb2.Set_font_color_pco(0);
          p1tb3.Set_font_color_pco(0);
          p1tb4.Set_font_color_pco(0);
          p1tb5.Set_font_color_pco(0);
          p1tb6.Set_font_color_pco(0);
        }
        else if (keyvalue == 50)
        {
          Serial.println("Zoom 2");
          p1tb1.Set_font_color_pco(0);
          // p1tb2.setFont(3);
          p1tb2.Set_font_color_pco(63488);
          p1tb3.Set_font_color_pco(0);
          p1tb4.Set_font_color_pco(0);
          p1tb5.Set_font_color_pco(0);
          p1tb6.Set_font_color_pco(0);
        }
        else if (keyvalue == 51)
        {
          p1tb1.Set_font_color_pco(0);
          p1tb2.Set_font_color_pco(0);
          p1tb3.Set_font_color_pco(63488);
          p1tb4.Set_font_color_pco(0);
          p1tb5.Set_font_color_pco(0);
          p1tb6.Set_font_color_pco(0);
        }
        else if (keyvalue == 52)
        {
          p1tb1.Set_font_color_pco(0);
          p1tb2.Set_font_color_pco(0);
          p1tb3.Set_font_color_pco(0);
          p1tb4.Set_font_color_pco(63488);
          p1tb5.Set_font_color_pco(0);
          p1tb6.Set_font_color_pco(0);
        }
        else if (keyvalue == 53)
        {
          p1tb1.Set_font_color_pco(0);
          p1tb2.Set_font_color_pco(0);
          p1tb3.Set_font_color_pco(0);
          p1tb4.Set_font_color_pco(0);
          p1tb5.Set_font_color_pco(63488);
          p1tb6.Set_font_color_pco(0);
        }
        else if (keyvalue == 54)
        {
          p1tb1.Set_font_color_pco(0);
          p1tb2.Set_font_color_pco(0);
          p1tb3.Set_font_color_pco(0);
          p1tb4.Set_font_color_pco(0);
          p1tb5.Set_font_color_pco(0);
          p1tb6.Set_font_color_pco(63488);
        }
      }
    }

    if(keyvalue == 35){
      secondPage++;
      if(secondPage == 6){
        secondPage=5;
      }
       int dividedValue = arrayconsumed[counter] / 5; // Divide the value by 5
    int firstDigit = dividedValue % 10; // Extract the first digit
    if(secondPage>firstDigit){
      secondPage--;
    }

      Serial.println("#######################################");
      Serial.println("MOVING TO NEXT PAGE IN SAME CATEGORY");
      Serial.println("COUNTER :"+(String)counter);
      Serial.println("PAGE : "+(String)+secondPage);
        Serial.println("CATEGORY SIZE : "+(String)+categorySize);
        Serial.println("this arary size: "+(String)+arrayconsumed[counter]);
        Serial.println("#######################################");
      if(counter == 0){
        showPage1Reason();
      }
       if (counter == 1)
        {
          showPage2Reason();
        }
       if (counter == 2)
        {
          showPage3Reason();
        }
       if (counter == 3)
        {
          showPage4Reason();
        }
       if (counter == 4)
        {
          showPage5Reason();
        }
      keyvalue=0;
      }

        if(keyvalue == 42){
       
      secondPage--;
      if(secondPage < 0){
secondPage=0;
      }
            Serial.println("#######################################");
    Serial.println("MOVING BACK IN SAME CATEGORY");
      Serial.println("COUNTER :"+(String)counter);
      Serial.println("PAGE : "+(String)+secondPage);
       Serial.println("this arary size: "+(String)+arrayconsumed[counter]);
        Serial.println("#######################################");
      
      if(counter == 0){
        showPage1Reason();
      }
       if (counter == 1)
        {
          showPage2Reason();
        }
       if (counter == 2)
        {
          showPage3Reason();
        }
       if (counter == 3)
        {
          showPage4Reason();
        }
       if (counter == 4)
        {
          showPage5Reason();
        }
      keyvalue=0;
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
Serial.println("INPUTTING KELOMETERS");
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

      if (inputKelo <= lastKeloUsed)
      {
        cpass += "Error Val";
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
  File root = SPIFFS.open("/uploadData");

  File file = root.openNextFile();

  while (file)
  {

    Serial.print("FILE: ");
    Serial.println(file.name());

    file = root.openNextFile();
  }
  Serial.println("######################  END SPIFF #######################");
}

void listMyMilage()
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

    if (postData(serverData) == 200)
      SPIFFS.remove(name);


      file = root.openNextFile();
  }
}

void resetAll()
{

  digitalWrite(pump_RELAY, LOW);
  // postFlag = false;
  delay(3000);

  counter = 0;
secondPage=0;  

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

void rememberKelomSpiff(String rfidnum, int kilom)
{
  Serial.println("Save Mileage in SPIFF");
  const char *jsonFilePath = "/rememberKelo.json";
  File jsonFile = SPIFFS.open(jsonFilePath, "r");
  if (jsonFile)
  {
    Serial.println("Found: " + (String)jsonFilePath);
    size_t fileSize = jsonFile.size();
    std::unique_ptr<char[]> jsonBuffer(new char[fileSize]);
    jsonFile.readBytes(jsonBuffer.get(), fileSize);

    // Parse the JSON data
    DynamicJsonDocument jsonData(1024);
    DeserializationError error = deserializeJson(jsonData, jsonBuffer.get());
    if (error)
    {
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
    if (jsonData.containsKey(rfidnum))
    {
      // Update the kilom value
      jsonData[rfidnum]["kilom"] = kilom;
    }
    else
    {
      // Add the RFID number with the kilom value
      jsonData[rfidnum]["kilom"] = kilom;
    }
    serializeJsonPretty(jsonData, Serial);
    // Save the updated JSON data to SPIFFS
    jsonFile = SPIFFS.open(jsonFilePath, "w");
    if (jsonFile)
    {
      serializeJson(jsonData, jsonFile);
      jsonFile.close();

      Serial.println((String)jsonFile.readString());
      Serial.println("JSON data saved successfully");
    }
    else
    {
      Serial.println("Failed to open JSON file for writing");
    }
  }
  else
  {
    Serial.println("Failed to open JSON file");
  }
}
void Task1code(void *pvParameters)
{
  // Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;)
  {

    loopNextion();
    readRFID();
    loopkeypad();
    handlingMenus();

if(millis() - showTimeMillis > 10000){
  String msg=dateStamp2+" "+timeStamp2+" WIFI";
  timeinfo.setText(msg.c_str());
   if (WiFi.status() != WL_CONNECTED){
 timeinfo.Set_font_color_pco(63488);
   }
 
  showTimeMillis=millis();
}

    if (millis() - prevMillisRef > 50000) // ReFRESH the menu no contact
    {
      resetAll();
      prevMillisRef = millis();
    }
    if ((userFlag == true) || (vechileFlag == true))
    {
      if ((millis() - flagcheckMillis < 15000) && (userFlag == true) && (vechileFlag == true))
      {
        Serial.println("BOTH ARE ASSIGNED");
        String msg="KM/H: "+ vechileArray[2];


        Serial2.print("page page1");
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);
        showPage1Reason();

        Serial.println("MAIN MENU WILL BE OPENED");
        getlastkm();
        // for (int i = 0; i < vechileRfidArray->length(); i++)
        // {
        //   Serial.println(i);
        //   Serial.println("VECHILE TAG : " + (String)vechileArray[0]);
        //   Serial.println("RFID TAG : " + (String)vechileRfidArray[i]);

        //   if (vechileArray[0] == vechileRfidArray[i])
        //   {
        //     Serial.print("############################################## RFID FOUND ###################################");
        //     Serial.println("LAST KELO USED BY THIS ID  : " + (String)keloArray[i]);

        //     String lastval = keloArray[i];
        //     // Serial.println("LATVALIE : "+(String)lastval);
        //     lastKeloUsed = lastval.toInt();
        //     exit;
        //   }
        // }

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

    // ##########  CONDITION FOR FLOW METER
    if (flowFlag == true)
    {
      value = CheckFlow(calibrationFactor);
      dtostrf(value, 5, 2, buffer); // Convert the float to a string with 5 digits, 2 decimal places, and store it in the buffer
      Serial.println(buffer);
      String a = buffer;
      p2tb1.setText(a.c_str());
    }

    if (resetFlag == true)
    {
      resetAll();
    }

    TIMERG1.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
    TIMERG1.wdt_feed = 1;
    TIMERG1.wdt_wprotect = 0;

    TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
    TIMERG0.wdt_feed = 1;
    TIMERG0.wdt_wprotect = 0;
  }
}

// Task2code: blinks an LED every 700 ms
void Task2code(void *pvParameters)
{
  // Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;)
  {
    readHC12();
    if (millis() - checkepoch > 60000 && WiFi.isConnected())
    {
      getEpoch();
      listMySpiff();
      uploadSpiffFile();
      if(getReasonFlag == true){
         getReasonFromSpiff();
         getReasonFlag=false;
      }
      checkepoch = millis();
    }
    if (millis() - prevMillRTC > 15000)
    {
      Serial.println(getEpochTimeMillis());
      showRTCdT();
      mfrc522.PCD_Init(); // RFID sensor initialized
      prevMillRTC = millis();
    }

    if (WiFi.isConnected() == false)
    {
      setupWifi();
    }

    if (postFlag == true)
    {
      String a = "POSTING TO SERVER";
      String postStr = "{\"staid\":\"" + (String)station_ID + "\",\"urfid\":\"" + (String)userArray[0] + "\",\"vrfid\":\"" + (String)vechileArray[0] + "\",\"rone\":\"" + (String)reasonArray[0] + "\",\"rtwo\":\"" + (String)reasonArray[1] + "\",\"rthree\":\"" + (String)reasonArray[2] + "\",\"rfour\":\"" + (String)reasonArray[3] + "\",\"rfive\":\"" + (String)reasonArray[4] + "\",\"km\":\"" + (String)inputKelo + "\",\"fuel\":\"" + (String)finalLiters + "\",\"epoch\":\"" + (String)getEpochTimeMillis() + "\"}";
      Serial.println("POSTING: " + postStr);
      flowFlag = false;
      // Saving last kelometers in spiff;
      rememberKelomSpiff(vechileArray[0], inputKelo);
inputKelo = 0;
      if (WiFi.status() != WL_CONNECTED)
      {
        String time = (String)getEpochTimeMillis();
        String fileName = "/uploadData/" + time + ".json";
        writeJsonToSpiff(fileName.c_str(), postStr.c_str());
        resetFlag = true;
        postFlag = false;
      }
      else
      {
        if (postData(postStr) == 200)
        {
          String a = "Sucessfully Posted";
          p2tb1.setText(a.c_str());
          resetFlag = true;
          postFlag = false;
          finalLiters = 0;
          value = 0;
        }
        else
        {
          String a = "Not Posted";
          p2tb1.setText(a.c_str());
          resetFlag = true;
          postFlag = false;

          String time = (String)getEpochTimeMillis();
          String fileName = "/uploadData/" + time + ".json";
          writeJsonToSpiff(fileName.c_str(), postStr.c_str());
        }
      }
    }

    TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
    TIMERG0.wdt_feed = 1;
    TIMERG0.wdt_wprotect = 0;

    TIMERG1.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
    TIMERG1.wdt_feed = 1;
    TIMERG1.wdt_wprotect = 0;
  }
}

void createJsonFile()
{
  if (!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS initialization failed");
    return;
  }

  String fileName = "/reason.json";
  //String fileName = "/data.json";

  if (SPIFFS.exists(fileName))
  {
    Serial.println("File already exists");
    SPIFFS.remove(fileName);
   // return;
  }

  File file = SPIFFS.open(fileName, "w");
  if (!file)
  {
    Serial.println("Failed to create file");
    return;
  }

  // JSON content to write
  const char* jsonContent = "[{\"id\":\"22\",\"staid\":\"2\",\"rfidnum\":\"20217812825\",\"usertyp\":\"Vechile\",\"name\":\"CAR\"},{\"id\":\"23\",\"staid\":\"2\",\"rfidnum\":\"1388167127\",\"usertyp\":\"User\",\"name\":\"ARSLAN\"},{\"id\":\"27\",\"staid\":\"2\",\"rfidnum\":\"289312259\",\"usertyp\":\"Vechile\",\"name\":\"JEEP\"},{\"id\":\"28\",\"staid\":\"2\",\"rfidnum\":\"144861132\",\"usertyp\":\"User\",\"name\":\"Marco\"},{\"id\":\"29\",\"staid\":\"2\",\"rfidnum\":\"133928\",\"usertyp\":\"Vehicle\",\"name\":\"HRT951FS\"}]";

  file.print(jsonContent);
  file.close();

  Serial.println("JSON File created successfully");
}

void setup()
{
  Serial.begin(115200);
//createJsonFile();
  setupHC();
  setupRTC();
  /* code */
  showRTCdT();

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
  Serial.println("###########   LISTING SPIFF MILAGE ################");
  listMyMilage();

  getlastkm();
  setupFlowSensor();
  getReasonFromSpiff();
  getMinMaxFromSpiff();
  setupWifi();
  if (WiFi.isConnected())
  {
    getntp();
    uploadSpiffFile();
    getEpoch();
    // unsigned long prevMillisep=millis();
    // while (epochTime < 1 && millis()-prevMillisep<3000)
    //     {
    //     getntp();
    //     }
    if (epochTime > 1684592279) // check for valid epoch
      setDateTimeFromEpoch(epochTime);
  }

  Serial.println("EPOUCH : " + (String)rtc.getEpoch());

  Serial.println("Approach your reader card...");
  Serial.println();
  Serial2.print("page page0");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  listMySpiff();

  setupRTC();
  showRTCdT();

  xTaskCreatePinnedToCore(
      Task1code, /* Task function. */
      "Task1",   /* name of task. */
      10000,     /* Stack size of task */
      NULL,      /* parameter of the task */
      1,         /* priority of the task */
      &Task1,    /* Task handle to keep track of created task */
      0);        /* pin task to core 0 */
  // delay(100);

  // create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
      Task2code, /* Task function. */
      "Task2",   /* name of task. */
      10000,     /* Stack size of task */
      NULL,      /* parameter of the task */
      1,         /* priority of the task */
      &Task2,    /* Task handle to keep track of created task */
      1);        /* pin task to core 1 */
  // delay(100);
}

void loop()
{
}