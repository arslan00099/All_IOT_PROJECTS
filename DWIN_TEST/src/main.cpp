
#include <Arduino.h>
#include <DWIN.h>
#include "rfidscanner.h"
#include "savedatatospiff.h"
#include <WiFi.h>
#include "index.html"
#include <Arduino_JSON.h>


const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char *ssid = "Neo";
const char *password = "12345678";



String outputState(int output){
  if(digitalRead(output)){
    return "checked";
  }
  else {
    return "";
  }
}

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons = "";
    return buttons;
  }
  return String();
}

JSONVar readings;
void clearData(int);
int customerID=0;
int finalItem=0;
int finalPrice=0;
bool finalPageFlag=false;


#define ADDRESS_A "1010"
#define ADDRESS_B "1020"

#define DGUS_BAUD 115200
const int arraySize = 11; // Size of your dataArray array
String dataArray[arraySize] = {"", "", "", "", "", "", "", "", "", "", ""};
int addressArray[arraySize] = {0x1000, 0x2000, 0x3000, 0x4000, 0x5000, 0x6000, 0x7000, 0x8000, 0x9000};
String rfidArray[arraySize]={"", "", "", "", "", "", "", "", "", "", ""};
int itemCount=0;
int totalBill=0;
bool found = false;
String productName;
int quantity=0;
int price=0;
int netBill;
const int buttonPin = 26;  // Pin number where your button is connected
unsigned long buttonPressStartTime = 0;
boolean buttonIsPressed = false;
int id=1;


// If Using ESP 32
#if defined(ESP32)
#define DGUS_SERIAL Serial2
DWIN hmi(DGUS_SERIAL, 16, 17, DGUS_BAUD);

// If Using Arduino Uno
#else
DWIN hmi(2, 3, DGUS_BAUD);
#endif

// Event Occurs when response comes from HMI
void onHMIEvent(String address, int lastByte, String message, String response)
{
  Serial.println("OnEvent : [ A : " + address + " | D : " + String(lastByte, HEX) + " | M : " + message + " | R : " + response + " ]");
  
}

String getReadings()
{

  readings["cid"] = (String)customerID;
  readings["totalitem"] = (String)finalItem;
  readings["price"] = (String)finalPrice;

  String jsonString = JSON.stringify(readings);
 // Serial.println(jsonString);
  return jsonString;
}

void resetTrolly(){
  for (int i = 0; i < arraySize; i++)
      {
        clearData(addressArray[i]);
        dataArray[i]="";
        rfidArray[i]="";
      }

      clearData(0X1100);
      clearData(0X1200);
      itemCount=0;
      totalBill=0;
      hmi.setPage(00);
      customerID=0;
  finalItem=0;
  finalPrice=0;
      id++;
}

//######################## SERVER CODE STARTS HERE ################################

void setupServer()
{  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

    server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String json = getReadings();
    request->send(200, "application/json", json);
    json = String(); });

  // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/DATA", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
    hmi.setPage(5);
    finalPageFlag=true;

  });

  // Start server
  server.begin();
  
}

//####################################################################################

void clearData(int address)
{
  hmi.setText(address, "                                                                                                                                                                          ");
}

void spiffCompare()
{
  if (!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS initialization failed!");
    return;
  }
 
  File file = SPIFFS.open("/data.json", "r"); // Replace with the actual file path
  if (!file)
  {
    Serial.println("Failed to open file");
    return;
  }

  // Parse the JSON data
  DynamicJsonDocument doc(2000); // Adjust the buffer size as needed
  DeserializationError error = deserializeJson(doc, file);

  if (error)
  {
    Serial.print("JSON parsing failed: ");
    Serial.println(error.c_str());
    return;
  }

  // Close the file
  file.close();

  // RFID number to search for
  String targetRFID = rfidSt; // Change this to the RFID you're looking for
  Serial.println("TAEGETRFID : " + (String)targetRFID);

//#######################   FOR SAME RFID ###################
  for (int i = 0; i < arraySize; i++) {
  if (rfidArray[i].equals(targetRFID)) {
    Serial.println("RFID FOUND AT :"+(String)i+" :: "+(String)rfidArray[i]);
    itemCount--;
    dataArray[i]="";
    rfidArray[i]="";
    rfidSt="";
    for (int j = i; j < arraySize - 1; j++) {
        dataArray[j] = dataArray[j + 1];
        rfidArray[j] = rfidArray[j + 1];
    }
     for (JsonVariant obj : doc.as<JsonArray>())
  {
    // Check if the RFID number exists as a key in the object
    if (obj.containsKey(targetRFID))
    {
      JsonObject productData = obj[targetRFID];
      Serial.println(productData["netPrice"].as<float>(), 2);
      totalBill=totalBill-productData["netPrice"].as<int>();
    }}
  // targetRFID="";
   clearData(0X1100);
  clearData(0X1200);
      
      hmi.setText(0X1100,(String)itemCount);
      hmi.setText(0X1200,(String)totalBill);
    for (int i = 0; i < arraySize; i++)
      {clearData(addressArray[i]);
        hmi.setText(addressArray[i], dataArray[i]);    
      }
 found=true;
  }
}

//################## FOR SAME PRODUCT #############################33
for (int i = 0; i < arraySize; i++) {
//  Serial.println("CHECKING FOR ARRAYSIZE");
     for (JsonVariant obj : doc.as<JsonArray>())
  {
    // Check if the RFID number exists as a key in the object
    if (obj.containsKey(targetRFID))
    {
      JsonObject productData = obj[targetRFID];
     // Serial.println(productData["netPrice"].as<float>(), 2);
      netBill=productData["netPrice"].as<int>();
      productName=productData["productName"].as<String>();
      
    }
    }

  if (dataArray[i].indexOf(productName)>-1) {
    Serial.println("SAME PRODUCT AT :"+(String)i+" :: "+(String)rfidArray[i]);
    itemCount++;
    dataArray[i]="";
    rfidArray[i]="";
    rfidSt="";
    for (int j = i; j < arraySize - 1; j++) {
        dataArray[j] = dataArray[j + 1];
        rfidArray[j] = rfidArray[j + 1];
    }

    for (int i = arraySize - 1; i >= 1; i--)
      {
        dataArray[i] = dataArray[i - 1];
        rfidArray[i]= rfidArray[i -1];
      }

        String command;
      command += "  ";
      command += (String)productName;
      command += "                ";
      command += (String)netBill;
      command += "                ";
      command += (String)2;
      command += "                ";
      command += (String)(netBill*2);
      command += "             ";

       dataArray[0] = command;
      rfidArray[0] =targetRFID;
  
   targetRFID="";
    clearData(0X1100);
   clearData(0X1200);
      totalBill=totalBill+netBill;
      hmi.setText(0X1100,(String)itemCount);
      hmi.setText(0X1200,(String)totalBill);
    for (int i = 0; i < arraySize; i++)
      {clearData(addressArray[i]);
        hmi.setText(addressArray[i], dataArray[i]);    
      }
 found=true;
  }
}



if(found==false){
  // Iterate through the JSON array
  for (JsonVariant obj : doc.as<JsonArray>())
  {
    // Check if the RFID number exists as a key in the object
    if (obj.containsKey(targetRFID))
    {
      itemCount++;
      // Print the product information for the matching RFID
      JsonObject productData = obj[targetRFID];
      Serial.print("RFID found: ");
      Serial.println(targetRFID);
      Serial.print("Product Name: ");
      Serial.println(productData["productName"].as<String>());
      Serial.print("Price: $");
      Serial.println(productData["price"].as<float>(), 2); // Display price with 2 decimal places
      Serial.print("Quantity: ");
      Serial.println(productData["quantity"].as<int>());
      Serial.print("Net Price: $");
      Serial.println(productData["netPrice"].as<float>(), 2);
      totalBill=totalBill+productData["netPrice"].as<int>();

      String command;
      command += "  ";
      command += (String)productData["productName"].as<String>();
      command += "                ";
      command += (String)productData["price"].as<String>();
      command += "                ";
      command += (String)productData["quantity"].as<String>();
      command += "                ";
      command += (String)productData["netPrice"].as<String>();
      command += "             ";

      for (int i = arraySize - 1; i >= 1; i--)
      {
        dataArray[i] = dataArray[i - 1];
        rfidArray[i]= rfidArray[i -1];
      }

      dataArray[0] = command;
      rfidArray[0] =targetRFID;
      Serial.println("SAVED VALUE : "+(String)rfidArray[0]);
      

      for (int i = 0; i < arraySize; i++)
      {
        clearData(addressArray[i]);
        Serial.println(dataArray[i]);
      }

      clearData(0X1100);
      clearData(0X1200);
      
      hmi.setText(0X1100,(String)itemCount);
      hmi.setText(0X1200,(String)totalBill);
      Serial.println("ITEM COUNT : "+(String)itemCount);

      for (int i = 0; i < arraySize; i++)
      {
        hmi.setText(addressArray[i], dataArray[i]);
       
      }

      rfidSt = "";
      break; // Exit the loop once a match is found
     }
  }
  }
}

void checkButton() {
  int buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) {
    hmi.setPage(04);
    // Button is pressed
    if (!buttonIsPressed) {
      // Record the start time when the button is first pressed
      buttonPressStartTime = millis();
      buttonIsPressed = true;
    }
    // Check if the button has been held for 5 seconds
    if (millis() - buttonPressStartTime >= 5000) {
     resetTrolly();
    }
  } else {
    // Button is released
    buttonIsPressed = false;
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin,INPUT_PULLUP);

  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }
//  if (SPIFFS.format()) {
//     Serial.println("SPIFFS formatted successfully");
//   } else {
//     Serial.println("SPIFFS format failed");
//   }
  saveData();
  readSpiff();

  setupRfid();
  Serial.println("DWIN HMI ~ Hello World");
  hmi.echoEnabled(false);
  hmi.hmiCallBack(onHMIEvent);

  


  Serial.println("PRINTED");
  byte a=hmi.getPage();
  Serial.print("PAGE ####");
// Serial.println(a);
//   delay(5000);
 
//   hmi.setPage(01);
//      a=hmi.getPage();
//    Serial.print("PAGE ####");
// Serial.println(a);

setupServer();

}

void loop()
{
  // Listen HMI Events
  checkButton();
  hmi.listen();
  readRFID();
  if (rfidSt.length() > 0)
  {found=false;
  hmi.setPage(1);
    spiffCompare();
  }

int page=hmi.getPage();
if(page == 3){
  customerID=id;
  finalItem=itemCount;
  finalPrice=totalBill;
}

if(finalPageFlag){
  delay(5000);
    resetTrolly();
    finalPageFlag=false;
}

}