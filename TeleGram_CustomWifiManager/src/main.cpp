#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "index.html"
#include <Preferences.h>
#include <Arduino_Json.h>
Preferences preferences;
JSONVar readings;

// Replace with your network credentials
const char *ssid = "RIGJAZZ";
const char *password = "Kashmiri786";

const char *PARAM_INPUT_1 = "output";
const char *PARAM_INPUT_2 = "state";
const char *http_username = "admin";
const char *http_password = "admin";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String SSID, PASS, deviceid, aliesname, oneon, oneoff, twoon, twooff, threeon, threeoff, fouron, fouroff, fiveon, fiveoff, sixon, sixoff, dlytime;

void loadPref()
{
  preferences.begin("my-app", false);
  SSID = preferences.getString("SSID");
  PASS = preferences.getString("PASSWORD");
  deviceid = preferences.getString("deviceid");
  aliesname = preferences.getString("alname");
  oneon = preferences.getString("oneon");
  oneoff = preferences.getString("oneoff");
  twoon = preferences.getString("twoon");
  twooff = preferences.getString("twooff");
  threeon = preferences.getString("threeon");
  threeoff = preferences.getString("threeoff");
  fouron = preferences.getString("fouron");
  fouroff = preferences.getString("fouroff");
  fiveon = preferences.getString("fiveon");
  fiveoff = preferences.getString("fiveoff");
  sixon = preferences.getString("sixon");
  sixoff = preferences.getString("sixoff");
  dlytime = preferences.getString("dlytime");
  Serial.println("Device id: " + (String)deviceid);
  Serial.println("Alies Name : " + (String)aliesname);
  Serial.println("ONE ON: " + (String)oneon);
  Serial.println("ONE OFF: " + (String)oneoff);
  Serial.println("TWO ON: " + (String)twoon);
  Serial.println("TWO OFF: " + (String)twooff);
  Serial.println("THREE ON: " + (String)threeon);
  Serial.println("THREE OFF: " + (String)threeoff);
  Serial.println("FOUR ON: " + (String)fouron);
  Serial.println("FOOUR OFF: " + (String)fouroff);
  Serial.println("FIVE ON: " + (String)fiveon);
  Serial.println("FIVE OFF: " + (String)fiveoff);
  Serial.println("SIX ON: " + (String)sixon);
  Serial.println("SIX OFF: " + (String)sixoff);
  Serial.println("DELAY TIME: " + (String)dlytime);
}

String getSensorReadings()
{

  readings["did"] = deviceid;
  readings["alias_name"] = aliesname;
  readings["oneoff"] = oneoff;
  readings["oneon"] = oneon;
  readings["twooff"] = twooff;
  readings["twoon"] = twoon;
  readings["threeoff"] = threeoff;
  readings["threeon"] = threeon;
  readings["fouroff"] = fouroff;
  readings["fiveon"] = fiveon;
  readings["fiveoff"] = fiveoff;
  readings["sixon"] = sixon;
  readings["sixoff"] = sixoff;
  readings["dlytime"] = dlytime;
  readings["ssid"] = SSID;
  readings["pass"] = PASS;
  String jsonString = JSON.stringify(readings);
  Serial.println(jsonString);
  return jsonString;
}

// Replaces placeholder with button section in your web page
String processor(const String &var)
{
  // Serial.println(var);
  if (var == "BUTTONPLACEHOLDER")
  {
    String buttons = "";
    return buttons;
  }
  return String();
}

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);
  loadPref();
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String json = getSensorReadings();
              request->send(200, "application/json", json);
              json = String();
            });

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html, processor); });
  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  //           {
  // if(!request->authenticate(http_username, http_password))
  //   return request->requestAuthentication();
  // request->send_P(200, "text/html", index_html, processor); });

  server.on("/json_open", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", json_html, processor); });
  server.on("/logged-out", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", logout_html, processor); 
            ESP.restart(); });

  server.on("/wifi_open", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", wifi_html, processor); });

  // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
     // digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    Serial.print("KEY : ");
    Serial.print(inputMessage1);
    Serial.print(" - VALUE : ");
    Serial.println(inputMessage2);
         preferences.begin("my-app", false);
     inputMessage2.trim();
        preferences.putString(inputMessage1.c_str(), inputMessage2);
        preferences.end();
    request->send(200, "text/plain", "OK"); });

  // Start server
  server.begin();
}

void loop()
{
}