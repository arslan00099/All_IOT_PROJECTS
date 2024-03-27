#include <Arduino.h>
#include <FS.h>
#include <WiFi.h>
#include "index.h"
#include <AsyncTCP.h>
#include <Arduino_JSON.h>
#include <Preferences.h>

#include <FastLED.h>
#define NUM_LEDS 30
#define DATA_PIN_left 4
#define DATA_PIN_right 5
#define leftButton_PIN 15
#define rightButton_PIN 13

CRGB leftLeds[NUM_LEDS];
CRGB rightLeds[NUM_LEDS];

Preferences preferences;

unsigned long laponepmillis = 0;

const char *PARAM_INPUT = "value";
const char *http_username = "admin";
const char *http_password = "admin";

JSONVar readings;

unsigned long previousmillis = 0;

const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";

const char *PARAM_INPUT_1 = "output";
const char *PARAM_INPUT_2 = "state";
int r,g,b;



String stripName;
String stripDirection;
String leftColor;
String rightColor;
String drlColor;
String mode;

AsyncWebServer server(80);

void getpreferences()
{
  preferences.begin("my-app", false);
  stripName = preferences.getString("strip");
  stripDirection = preferences.getString("neo");
  leftColor = preferences.getString("leftneoclr");
  rightColor = preferences.getString("rightneoclr");
  mode=preferences.getString("mode");
  drlColor = preferences.getString("drlcolor");
  // Serial.println(stripName);
  // Serial.println(stripDirection);
  // Serial.println(leftColor);
  // Serial.println(rightColor);
  preferences.end();
}

String processor(const String &var)
{

  if (var == "TEST")
  {
    String buttons = "";
    buttons += (String) "test";
    return buttons;
  }

  return String();
}

String getSensorReadings()
{

  readings["radio"] = (stripName);

  String jsonString = JSON.stringify(readings);
  Serial.println(jsonString);
  return jsonString;
}

void leftColorCheck()
{
 
  // if ((stripName.indexOf("ws2812b") > -1))
  // {
    Serial.println("LEFT STRIP");
    String hexstring = "#" + (String)leftColor;
    int number = (int)strtol(&hexstring[1], NULL, 16);
    int r = number >> 16;
    int g = number >> 8 & 0xFF;
    int b = number & 0xFF;

    for (int i = 0; i < 30; i++)
    {
      leftLeds[i] = CRGB(r, g, b);
      FastLED.show();
      delay(10);
    }

    for (int i = 0; i < 30; i++)
    {
      leftLeds[i] = CRGB(000, 000, 000);
      FastLED.show();
        }
  
}
   void rightColorCheck(){
    Serial.println("RIGHT STRIP");
    String    hexstring = "#" + (String)rightColor;

    int  number = (int)strtol(&hexstring[1], NULL, 16);

     r = number >> 16;
     g = number >> 8 & 0xFF;
     b = number & 0xFF;

    for (int i = 0; i < 30; i++)
    {
      rightLeds[i] = CRGB(r, g, b);
      FastLED.show();
      delay(10);
    }

    for (int i = 0; i < 30; i++)
    {
      rightLeds[i] = CRGB(000, 000, 000);
      FastLED.show();
      
    }
  
  }
 
void ldrCheck(){
 // if ((stripName.indexOf("ws2812b") > -1) && (mode.indexOf("drl") > -1))
  
      Serial.println("DRL");
      String hexstring = "#" + (String)drlColor;
      int number = (int)strtol(&hexstring[1], NULL, 16);
      int r = number >> 16;
      int g = number >> 8 & 0xFF;
      int b = number & 0xFF;
      for (int i = 0; i < 30; i++)
      {
        leftLeds[i] = CRGB(r, g, b);
        rightLeds[i] = CRGB(r, g, b);
        FastLED.show();
      }

 
}

void setupWifiM()
{

  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
  if(!request->authenticate(http_username, http_password))
    return request->requestAuthentication();
  request->send_P(200, "text/html", index_html, processor); });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)

            { request->send_P(200, "text/html", index_html, processor); });
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String json = getSensorReadings();
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
                   Serial.println(inputMessage2);

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
            } 
            );
  

  server.begin();
}

void setup()
{
  Serial.begin(115200);
  pinMode(leftButton_PIN,INPUT_PULLUP);
  pinMode(rightButton_PIN, INPUT_PULLUP);
  getpreferences();
  Serial.println(stripName);
  setupWifiM();
  if (stripName.indexOf("ws2812b") > -1)
  {
    FastLED.addLeds<NEOPIXEL, DATA_PIN_left>(leftLeds, NUM_LEDS);
    FastLED.addLeds<NEOPIXEL, DATA_PIN_right>(rightLeds, NUM_LEDS);
  }
  else {
  FastLED.addLeds<SK6812, DATA_PIN_left, RGB>(leftLeds, NUM_LEDS);
  FastLED.addLeds<SK6812, DATA_PIN_right, RGB>(rightLeds, NUM_LEDS);
  }
}


void loop()
{
  if (millis() - previousmillis>2000){
    // leftColorCheck();
    // rightColorCheck();
int leftbtnValue=(digitalRead(leftButton_PIN));
int rightbtnValue=(digitalRead(rightButton_PIN));
if (leftbtnValue==false || rightbtnValue==false)
{
  if (leftbtnValue == 0)
  {
    leftColorCheck();
  }

if (rightbtnValue == 0)
{
  rightColorCheck();
}
}
else
{
  ldrCheck();
}


    previousmillis = millis();
  }
 
}
  