#include "index.h"


Preferences preferences;
JSONVar readings;


// Replace with your network credentials
const char* ssid = "Neo";
const char* password = "12345678";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";
String ssidp;
String passwordp;












void getpreferences()
{
  preferences.begin("my-app", false);
  ssidp = preferences.getString("SSID");
  passwordp = preferences.getString("PASSWORD");
  preferences.end();
}

String getssidandpassword()
{
  readings["ssid"] = (ssidp);
  readings["pass"] = (passwordp);
  readings["ip"]= WiFi.localIP().toString();
  
  String jsonString = JSON.stringify(readings);
  Serial.println(jsonString);
  return jsonString;
}

String getnames(){
   preferences.begin("my_preferences", false);
  String jsonData = preferences.getString("json_data", "");
   return jsonData;
}

String getButtonStatus(){
 preferences.begin("my-app", true);  // Open preferences for reading

  JSONVar jsonObject;
  jsonObject["ANTENNA1"] = preferences.getInt("ANTENNA1", 0);
  jsonObject["ANTENNA2"] = preferences.getInt("ANTENNA2", 0);
  jsonObject["ANTENNA3"] = preferences.getInt("ANTENNA3", 0);
  jsonObject["ANTENNA4"] = preferences.getInt("ANTENNA4", 0);
  jsonObject["ANTENNA5"] = preferences.getInt("ANTENNA5", 0);

  String jsonString = JSON.stringify(jsonObject);
  Serial.println(jsonString);
  preferences.end();
  return jsonString;
}


void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  getpreferences();
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  if(!SPIFFS.begin()){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/wifi_open", HTTP_GET, [](AsyncWebServerRequest *request)
  { request->send_P(200, "text/html", wifi_html); 
  });

    server.on("/names", HTTP_GET, [](AsyncWebServerRequest *request)
  { request->send_P(200, "text/html", names_html); 
  });
  
  server.on("/bg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/bg.jpg", "image/jpg");
  });

    server.on("/back", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/back.png", "image/png");
  });

    server.on("/setting", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/setting.png", "image/png");
  });

    server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String json = getssidandpassword();
    request->send(200, "application/json", json);
    json = String(); });

     server.on("/getnames", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String json = getnames();
    request->send(200, "application/json", json);
    json = String(); });

    server.on("/getbuttonstatus", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String json = getButtonStatus();
    request->send(200, "application/json", json);
    json = String(); });

   server.on("/DATA", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String inputMessage1;
              String inputMessage2;

            
              if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2))
              {
                inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
                inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
               
                if (inputMessage1.indexOf("names") > -1)
                {
                preferences.begin("my-app", false);
                preferences.putString("json_data", inputMessage2);
                preferences.end();
                Serial.println("NAMES UPDATED");
                }


    if (inputMessage2.indexOf("ANTENNA1") > -1){
             preferences.begin("my-app", false);
              preferences.putInt(inputMessage2.c_str(), 1);
              preferences.putInt("ANTENNA2", 0);
              preferences.putInt("ANTENNA3", 0);
              preferences.putInt("ANTENNA4", 0);
               preferences.putInt("ANTENNA5", 0);
              preferences.end();  
            
    }
   if (inputMessage2.indexOf("ANTENNA2") > -1){
              preferences.begin("my-app", false);
              preferences.putInt("ANTENNA1", 0);
              preferences.putInt(inputMessage2.c_str(), 1);
              preferences.putInt("ANTENNA3", 0);
              preferences.putInt("ANTENNA4", 0);
               preferences.putInt("ANTENNA5", 0);
              preferences.end();  
  }
   if (inputMessage2.indexOf("ANTENNA3") > -1){
              preferences.begin("my-app", false);
              preferences.putInt("ANTENNA1", 0);
              preferences.putInt("ANTENNA2", 0);
              preferences.putInt(inputMessage2.c_str(), 1);
              preferences.putInt("ANTENNA4", 0);
               preferences.putInt("ANTENNA5", 0);
              preferences.end();

  }
   if (inputMessage2.indexOf("ANTENNA4") > -1){
              preferences.begin("my-app", false);
              preferences.putInt("ANTENNA1", 0);
              preferences.putInt("ANTENNA2", 0);
              preferences.putInt("ANTENNA3", 0);
              preferences.putInt(inputMessage2.c_str(), 1);
               preferences.putInt("ANTENNA5", 0);
              preferences.end();

  }
   if (inputMessage2.indexOf("ANTENNA5") > -1){
              preferences.begin("my-app", false);
              preferences.putInt("ANTENNA1", 0);
              preferences.putInt("ANTENNA2", 0);
              preferences.putInt("ANTENNA3", 0);
              preferences.putInt("ANTENNA4", 0);
              preferences.putInt(inputMessage2.c_str(), 1);
              preferences.end();
              Serial.println("Antena 5");
  }



              }
              else
              {
                inputMessage1 = "No message sent";
                inputMessage2 = "No message sent";
              }
              Serial.print("KEY: ");
              Serial.print(inputMessage1);
              Serial.print(" Data: ");
              Serial.println(inputMessage2);
              request->send(200, "text/plain", "OK");
              preferences.begin("my-app", false);
              preferences.putString(inputMessage1.c_str(), inputMessage2);
              preferences.end();
              getpreferences();
            });


  server.begin();
}
 
void loop(){
  
}