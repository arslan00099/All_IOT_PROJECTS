#include <Arduino.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "Servo.h"
#include "key.h"
#include <Arduino_JSON.h>

// Replace with your network credentials
const char *ssid = "Test";
const char *password = "12345678";

// Setting LED GPIO
const int ledPin = 2;

// Setting Servo PIN

// Setting door lock timer

unsigned long doorUnlockTime;

const char *PARAM_INPUT = "state";

// AsyncWebServer object on port 80
AsyncWebServer server(80);

// Add a new global variable at the top of the code to store the start time for the timer:
const char *PARAM_INPUT_1 = "output";
const char *PARAM_INPUT_2 = "state";

JSONVar readings;

String getReadings()
{

  readings["pass"] = (pass);

  String jsonString = JSON.stringify(readings);
  Serial.println(jsonString);
  return jsonString;
}

void setup()
{

  Serial.begin(115200);
  loadpref();
  pinMode(ledPin, OUTPUT);
  // Initialize SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to WiFi Succesfully");

  // Print ESP32 Local IP Address
  Serial.print("Please enter the following IP Adress into your web browser: ");
  Serial.println(WiFi.localIP());

  servo1.attach(servoPin);

  // For root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", "text/html"); });

  // To load style.css file
  //  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
  //    request->send(SPIFFS, "/style.css", "text/css");
  //  });

  // server.on("/value", HTTP_GET, [](AsyncWebServerRequest * request) {
  //   digitalWrite(ledPin, HIGH);
  //   servo1.write(180);
  //   Serial.println(request->url());
  //   request->send(200, "text/plain", "Hello from ESP32");
  //   // Record the start time for the timer
  //   startTime = millis();
  //   // Set lockDoor to true
  //   lockDoor = true;
  // });

  // // Route to set GPIO to HIGH
  // server.on("/180", HTTP_GET, [](AsyncWebServerRequest * request) {
  //   digitalWrite(ledPin, HIGH);
  //   servo1.write(180);
  //   Serial.println(request->url());
  //   request->send(200, "text/plain", "Hello from ESP32");
  //   // Record the start time for the timer
  //   startTime = millis();
  //   Serial.println("LOCK");
  //   // Set lockDoor to true
  //   lockDoor = true;
  // });

  // // Route to set GPIO to LOW
  // server.on("/0", HTTP_GET, [](AsyncWebServerRequest * request) {
  //   digitalWrite(ledPin, LOW);
  //   servo1.write(0);
  //   Serial.println(request->url());
  //   request->send(200, "text/plain", "Hello from ESP32");
  //   // Set lockDoor to false
  //     Serial.println("UNLOCK");
  //   lockDoor = false;
  // });

  // server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
  //   Serial.println(request->url());
  //   String inputMessage1;
  //   if (request->hasParam(PARAM_INPUT)) {
  //     inputMessage1 = request->getParam(PARAM_INPUT)->value();
  //     Serial.println(inputMessage1);
  //     servo1.write(inputMessage1.toInt());
  //   } else {
  //     inputMessage1 = "No message sent";
  //   }
  // });

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
              }
              else
              {
                inputMessage1 = "No message sent";
                inputMessage2 = "No message sent";
              }

              if (inputMessage1.indexOf("pass") > -1)
              {
                Serial.print("KEY: ");
                Serial.print(inputMessage1);
                Serial.print(" Data: ");
                Serial.println(inputMessage2);
                request->send(200, "text/plain", "OK");
                preferences.begin("my-app", false);
                preferences.putString(inputMessage1.c_str(), inputMessage2);
                preferences.end();
                loadpref();
              }

              else if (inputMessage1.indexOf("open") > -1)
              {
                startTime = millis();
                servo1.write(180);
                Serial.println("DOOR OPEN");
                // Set lockDoor to true
                lockDoor = true;
              }
              else if (inputMessage1.indexOf("close") > -1)
              {
                startTime = millis();
                servo1.write(0);
                Serial.println("DOOR CLOSE");
                // Set lockDoor to true
                lockDoor = false;
              }
            });

  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String json = getReadings();
    request->send(200, "application/json", json);
    json = String(); });

  // Start server
  server.begin();
}

void loop()
{
  loopkeypad();

  if ((lockDoor == true) && (millis() - startTime >= 10000))
  {
    // Lock the door
    digitalWrite(ledPin, LOW);
    servo1.write(0);
    lockDoor = false;
  }
}
