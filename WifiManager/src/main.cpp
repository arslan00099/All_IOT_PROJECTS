#include <Arduino.h>
#include <WiFi.h>
#include "index.html"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include <Preferences.h>
#include "sonar.h"


Preferences preferences;
float sonarreading;
double lapone;
unsigned long laponepmillis=0;
unsigned long laptwopmillis = 0;

const char *PARAM_INPUT = "value";
const char *http_username = "admin";
const char *http_password = "admin";

JSONVar readings;
JSONVar winner;
JSONVar lapstimeone;
JSONVar lapstimetwo;
unsigned long previousmillis = 0;
int sumcaronelap=0;
int sumcartwolap=0;

const char *ssid = "RIGJAZZ";
const char *password = "Kashmiri786";
// const char *ssid = "ZONG4G-D027";
// const char *password = "02756146";

const char *PARAM_INPUT_1 = "output";
const char *PARAM_INPUT_2 = "state";

String ssidp;
String passwordp;
String D1name;
String D2name;
String D1Cname;
String D2Cname;
String lapcount;
int anArray[20];
int arrayIndex = 0;
int anArraytwo[20];
int arrayIndextwo = 0;

AsyncWebServer server(80);

void getpreferences()
{
  preferences.begin("my-app", false);
  ssidp = preferences.getString("SSID");
  passwordp = preferences.getString("PASSWORD");
  D1name = preferences.getString("D1name");
  D2name = preferences.getString("D2name");
  D1Cname = preferences.getString("D1Cname");
  D2Cname = preferences.getString("D2Cname");
  lapcount = preferences.getString("lapscount");
  Serial.println(lapcount);

  preferences.end();
}


String processor(const String &var)
{
 
  if (var == "TEST")
  {
    String buttons = "";
    buttons += (String)"test";
    return buttons;
  }

  return String();
}

String readlapsone()
{
  for (int i = 0; i < arrayIndex; i++)
  {
    // Serial.print("LAP");
    // Serial.print(i);
    // Serial.print(":");
    // Serial.println(anArray[i]);
    lapstimeone["lap" + (String)i] = (String)(anArray[i]);
  }
  String jsonString = JSON.stringify(lapstimeone);
  Serial.println(jsonString);
  return jsonString;

  //return String(lapone);
}

String readlapstwo()
{
  for (int i = 0; i < arrayIndextwo; i++)
  {
    // Serial.print("LAP");
    // Serial.print(i);
    // Serial.print(":");
    // Serial.println(anArray[i]);
    lapstimetwo["lapt" + (String)i] = (String)(anArraytwo[i]);
  }
  String jsonString = JSON.stringify(lapstimetwo);
  Serial.println(jsonString);
  return jsonString;

  // return String(lapone);
}

String getSensorReadings()
{

  readings["D1name"] = (D1name);
  readings["D2name"] = (D2name);
  readings["ssid"] = (ssidp);
  readings["pass"] = (passwordp);
  readings["D1Cname"] = String(D1Cname);
  readings["D2Cname"] = String(D2Cname);
  readings["lapcount"] = String(lapcount);
  String jsonString = JSON.stringify(readings);
  Serial.println(jsonString);
  return jsonString;
}

String getwinner()
{
  Serial.println("lap one total : "+(String)sumcaronelap);
  Serial.println("lap two total : " + (String)sumcartwolap);
  if (sumcaronelap > sumcartwolap)
  {
    winner["winner"] = String("CAR ONE");
    String jsonString = JSON.stringify(winner);
    Serial.println(jsonString);
    return jsonString;
}
else if (sumcaronelap < sumcartwolap)
{
  winner["winner"] = String("CAR TWO");
  String jsonString = JSON.stringify(winner);
  Serial.println(jsonString);
  return jsonString;
}
}

void setup()
{
  Serial.begin(115200);
  getpreferences();

  setupSonar();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  
  Serial.println(WiFi.localIP());

  

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
  if(!request->authenticate(http_username, http_password))
    return request->requestAuthentication();
  request->send_P(200, "text/html", index_html, processor); });

  server.on("/logout", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(401); });

  server.on("/logged-out", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", logout_html, processor); });
  // server.on("/lapsone", HTTP_GET, [](AsyncWebServerRequest *request)
  //           { request->send_P(200, "text/plain", readlapsone().c_str()); });

  server.on("/wifi_open", HTTP_GET, [](AsyncWebServerRequest *request)

  { request->send_P(200, "text/html", wifi_html, processor); });

  server.on("/insta_open", HTTP_GET, [](AsyncWebServerRequest *request)
  { request->send_P(200, "text/html", insta_html, processor); });

  server.on("/json_open", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", json_html, processor); });
  server.on("/winner", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String json = getwinner();
    request->send(200, "application/json", json);
    json = String(); });

  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String json = getSensorReadings();
    request->send(200, "application/json", json);
    json = String(); });

  server.on("/lapsone", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String json = readlapsone();
    request->send(200, "application/json", json);
    json = String(); });

  server.on("/lapstwo", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String json = readlapstwo();
    request->send(200, "application/json", json);
    json = String(); });

  server.on("/DATA", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String inputMessage1;
              String inputMessage2;

              // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
              if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2))
              {
                inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
                inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
                if (inputMessage2.indexOf("restart") > -1)
                {
                  ESP.restart();
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

void loop()
{
  sonarreading=getSonarReadings();
  if(millis()-previousmillis>200){
 
   // Serial.println(btnState);
  //  Serial.println(sonarreading);
    if (sonarreading < 7)
    {
      Serial.println("car 1");
      if (arrayIndex < lapcount.toInt())
      {
        unsigned long time = millis() - laponepmillis;
        time = time / 1000.0;
        anArray[arrayIndex] = time;
     
        sumcaronelap += time;
      
      arrayIndex++;
      laponepmillis=millis();
      }
  }
  else if (sonarreading < 12 && sonarreading > 5)
  {
    Serial.println("car 2");
    if (arrayIndextwo < lapcount.toInt()){
      unsigned long time = millis() - laptwopmillis;
    time = time / 1000.0;
    anArraytwo[arrayIndextwo] = time;
    sumcartwolap += time;
    arrayIndextwo++;
    laptwopmillis = millis();
  }
  }
 
    previousmillis = millis();
  }
}
