
#include <WiFi.h>
#include "index.html"
// #include <AsyncTCP.h>
// #include <ESPAsyncWebServer.h>
String serverRfid;
boolean enrolFlag=false;
String inputId;

const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


int getIDFromRFID(String filePath, String rfidNumber) {
  int id = -1;

  // Mount the SPIFFS file system
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount SPIFFS");
    return id;
  }

  // Open the file for reading
  File file = SPIFFS.open(filePath, "r");
  if (!file) {
    Serial.println("Failed to open file");
    return id;
  }

  // Read the file into a String
  String fileData = file.readString();

  // Close the file
  file.close();

  // Parse the JSON data
  StaticJsonDocument<256> jsonDocument;
  DeserializationError error = deserializeJson(jsonDocument, fileData);

  if (error) {
    Serial.println("Failed to parse JSON");
    return id;
  }

  // Search for the ID based on the RFID number
  for (JsonPair keyValue : jsonDocument.as<JsonObject>()) {
    JsonObject entry = keyValue.value().as<JsonObject>();
   String rfid = entry["rfidnum"];

    if (strcmp(rfid.c_str(), rfidNumber.c_str()) == 0) {
      id = atoi(keyValue.key().c_str());
      Serial.println("ID FOUND : "+(String)id);
      break;
    }
  }

  return id;
}


String postCurrentData(){

  // Retrieve the ID based on the RFID number
  const char* filePath = "/data.json";
  String rfidNumber = "1388167127";
  Serial.println(" RFID LEN : "+(String)lastscanid.length());
  if(lastscanid.length() >4){

  int id = getIDFromRFID(filePath, lastscanid);

  if (id != -1) {
    Serial.print("ID found: ");
    Serial.println(id);
    String a=(String)id +"    "+(String)lastscanid;
     String res ="{\"data\":\""+a+"\"}";
    Serial.println("res :  "+res);
    return res;
  } else {
    Serial.println("ID not found");
      String res ="{\"data\":\"Not Found\"}";
    return res;
  }
  }

 String res ="{\"data\":\"No  ID Scanned\"}";
    return res;

}




String readDataFromFile(const char* filePath) {
  String data;

  // Mount the SPIFFS file system
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount SPIFFS");
    return data;
  }

  // Open the file for reading
  File file = SPIFFS.open(filePath, "r");
  if (!file) {
    Serial.println("Failed to open file");
    return data;
  }

  // Read data from the file into a String
  data = file.readString();
Serial.println("DATA : "+(String)data);

  // Parse JSON string into a DynamicJsonDocument
  DynamicJsonDocument jsonDocument(2048);
  DeserializationError error = deserializeJson(jsonDocument, data);

  if (error) {
    Serial.print("JSON parsing failed: ");
    Serial.println(error.c_str());
    return error.c_str();
  }

  // Remove the "staid" key
  jsonDocument.remove("staid");

  // Serialize the modified JSON document back to a string
  String outputJson;
  serializeJson(jsonDocument, outputJson);

  // Print the output JSON string
  Serial.println("AFTER REMOVING STA_ID : "+(String) outputJson);

  // Close the file
  file.close();

  return outputJson;
}

String getSensorReadings()
{
loadPreferences();
  String jsonString = "{\"ssid\":\""+ssid+"\",\"pass\":\""+password+"\"}";
Serial.println(jsonString);
  return jsonString;
}

String sendRFID()
{

  String jsonString = "{\"scnid\":\""+(String)lastscanid+"\",\"rfid\":\""+(String)serverRfid+"\"}";
  Serial.println(jsonString);
  return jsonString;
}



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
    buttons += "<h4>Output - GPIO 2</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" " + outputState(2) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Output - GPIO 4</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\" " + outputState(4) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Output - GPIO 33</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"33\" " + outputState(33) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}



void setupServer(){

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

    server.on("/record_open", HTTP_GET, [](AsyncWebServerRequest *request)
  { request->send_P(200, "text/html", showrecord_html, processor); });

  server.on("/enrol_open", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", enrol_html, processor); });

              server.on("/wifi_open", HTTP_GET, [](AsyncWebServerRequest *request)

  { request->send_P(200, "text/html", wifi_html, processor); });

  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String json = getSensorReadings();
    request->send(200, "application/json", json);
    json = String(); });

      server.on("/getRecord", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String json = readDataFromFile("/data.json");
    request->send(200, "application/json", json);
    json = String(); });

        server.on("/currentValue", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String json = postCurrentData();
    request->send(200, "application/json", json);
    json = String(); });

     server.on("/getRfid", HTTP_GET, [](AsyncWebServerRequest *request)
            {Serial.println("Geeting rfid from server");
    String json = sendRFID();
    request->send(200, "application/json", json);
    json = String(); });
  

  // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
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
    Serial.print("KEY: ");
    Serial.print(inputMessage1);
    Serial.print(" - DATA: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");

    if(inputMessage1.indexOf("keyid") > -1){
        inputId=inputMessage2;
        enrolFlag = true;
        Serial.println("Message Recieved");
    }
    else{
          preferences.begin("my-app", false);
              preferences.putString(inputMessage1.c_str(), inputMessage2);
              preferences.end();
              Serial.println("###########SAVED IN PREFERECES#############");
              loadPreferences();
                  if (SPIFFS.exists("/data.json"))
    {
      Serial.println("File /data.json exists");
    }
    else
    {
      Serial.println("File /data.json does not exist");
      if ((ssid.length() > 1) && (password.length() > 1))
      {
        preferences.begin("my-app", false);
        preferences.putInt("rst", 2); // if 1 then post data if 0 then open AP mode
        preferences.end();
        Serial.println("SWITCHING FROM 0 to 2");
        delay(5000);
        ESP.restart();
      }
    }
    }
  });

  // Start server
  server.begin();
}

