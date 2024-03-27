#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <TaskScheduler.h>
#include <ArduinoJson.h>
#include <stdio.h>
#include <FS.h>

AsyncWebServer server(80);

const char* ssid = "Neo";
const char* password = "12345678";


const size_t capacity = JSON_OBJECT_SIZE(40);
DynamicJsonDocument doc(capacity);
String dataReceived = "";
int button1 = 14;
int button2 = 27;
int button3 = 26;
int button4 = 25;
int electricPulse_one = 2;
int electricPulse_two = 15;

int keyUser;
int button1State = 0;
int button2State = 0;
int button3State = 0;
int button4State = 0;
int button1CState = 0;
int button2CState = 0;
int button3CState = 0;
int button4CState = 0;

char button1type = 'a';
char button2type = 'a';
char button3type = 'a';
char button4type = 'a';

const int nButtons = 4;
long to1 = 0;
long to2 = 0;
long to3 = 0;
String b1PW = "9999991";
String b2PW = "9999991";
String b3PW = "9999991";
String b4PW = "9999991";

//################ ARSLAN CODE #############
unsigned long lastChangeTime[4] = {0};
const int numGPIOs = 4; 
int leds[numGPIOs] = {14, 27, 26, 25};
bool pulseOneFlag=false;
bool pulseTwoFlag=false;


const char* users[][2] = {
  { "user1", "1234" },
  { "user2", "5678" },
  { "user3", "9012" },
};

String sendSystemState();
bool parseUserData(const String& data, String& username, String& password);
bool isValidUser(const char* username, const char* password);

int creatUser();
bool checkUsers();
void sendSSE(AsyncWebServerRequest *request, const String &data);

int getLedIndex(int button) {
  for (int i = 0; i < numGPIOs; i++) {
    if (leds[i] == button) {
      return i;
    }
  }
  return -1; // Invalid index
}

bool canChangeState(int button) {
  int index = getLedIndex(button);
  if (index != -1) {
    unsigned long currentTime = millis();
    return (currentTime - lastChangeTime[index]) > 5000; // Check if 5 seconds have passed
  }
  return false; // Invalid button
}

bool isValidGPIO(int button) {
  for (int i = 0; i < numGPIOs; i++) {
    if (leds[i] == button) {
      return true;
    }
  }
  return false;
}


void setup() {
  Serial.begin(115200);

  pinMode(button1, OUTPUT);
  pinMode(button2, OUTPUT);
  pinMode(button3, OUTPUT);
  pinMode(button4, OUTPUT);
  pinMode(electricPulse_one, OUTPUT);
  pinMode(electricPulse_two, OUTPUT);

  digitalWrite(button1, LOW);
  digitalWrite(button2, LOW);
  digitalWrite(button3, LOW);
  digitalWrite(button4, LOW);
  digitalWrite(electricPulse_one, LOW);
  digitalWrite(electricPulse_two, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    String htmlContent = "<html><body>";
    htmlContent += "<p>Please Enter Credentials or check states</p>";
    htmlContent += "<a href='/buttons'>Check the states of the system</a>";
    htmlContent += "</body></html>";

    request->send(200, "text/html", htmlContent);
  });

  server.on("/buttons", HTTP_GET, [](AsyncWebServerRequest* request) {
    if (request->hasParam("check_word")) {
      String checkWord = request->getParam("check_word")->value();

      if (checkWord == users[0][0]+b1PW ||checkWord == users[1][0]+b2PW ||checkWord == users[2][0]+b3PW) {
        String json = sendSystemState();

        request->send(200, "application/json", json);
        Serial.println("Button 1 toggled successfully");
      } else {
        request->send(400, "text/plain", "Invalid check_word");
      }
    } else {
      request->send(400, "text/plain", "Missing check_word parameter");
    }
  });

    server.on("/getbuttonstatus", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String json = sendSystemState();
    request->send(200, "application/json", json);
    json = String(); });

  server.on("/login", HTTP_POST, [](AsyncWebServerRequest* request) {
    String username;
    String password;

    if (request->hasParam("username", true) && request->hasParam("password", true)) {
      AsyncWebParameter* usernameParam = request->getParam("username", true);
      username = usernameParam->value();

      AsyncWebParameter* passwordParam = request->getParam("password", true);
      password = passwordParam->value();

      Serial.println("Received username: " + username);
      Serial.println("Received password: " + password);

      if (isValidUser(username.c_str(), password.c_str())) {
        keyUser = creatUser();
        if (strcmp(username.c_str(), users[0][0]) == 0) {
          b1PW = String(keyUser);
          Serial.print("user 1 new pw");
        } else if (strcmp(username.c_str(), users[1][0]) == 0) {
          Serial.print("user 2 new pw");
          b2PW = String(keyUser);
        } else if (strcmp(username.c_str(), users[2][0]) == 0) {
          Serial.print("user 3 new pw");
          b3PW = String(keyUser);
        }

        Serial.println("Login successful");
        String json1 = sendSystemState();
        String json = String("{\"key\":\"") + String(keyUser) + String("\",\"nButtons\":\"") + String(nButtons) + String("\"}") + "\n" + json1;

        doc.clear();
        DynamicJsonDocument doc(100);
        doc["key"] = keyUser;
        JsonObject stateObject = doc.createNestedObject("Buttons");

        stateObject["button1"] = button1State;
        stateObject["button2"] = button2State;
        stateObject["button3"] = button3State;
        stateObject["button4"] = button4State;
        String jsonString;
        serializeJson(doc, jsonString);

        request->send(200, "application/json", jsonString);
      } else {
        request->send(401, "text/plain", "Invalid username or password");
        Serial.println("Invalid username or password");
      }
    } else {
      request->send(400, "text/plain", "Invalid data format. Make sure to provide both username and password.");
      Serial.println("Invalid data format");
    }
  });

  server.on("/button1", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("check_word")) {
      String checkWord = request->getParam("check_word")->value();

      if (checkWord == users[0][0]+b1PW ||checkWord == users[1][0]+b2PW ||checkWord == users[2][0]+b3PW) {
        button1State = 1 - button1State;
        String json = sendSystemState();
        request->send(200, "application/json", json);
        Serial.println("Button 1 toggled successfully");
      } else {
        request->send(400, "text/plain", "Invalid check_word");
      }
    } else {
      request->send(400, "text/plain", "Missing check_word parameter");
    }
  });

  server.on("/button1Contact", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("check_word")) {
      String checkWord = request->getParam("check_word")->value();

      if (checkWord == users[0][0]+b1PW ||checkWord == users[1][0]+b2PW ||checkWord == users[2][0]+b3PW) {
        button1CState = 1 - button1CState;
        String json = sendSystemState();
        request->send(200, "application/json", json);
        Serial.println("Button 1 toggled successfully");
      } else {
        request->send(400, "text/plain", "Invalid check_word");
      }
    } else {
      request->send(400, "text/plain", "Missing check_word parameter");
    }
  });

  server.on("/button2", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("check_word")) {
      String checkWord = request->getParam("check_word")->value();

      if (checkWord == users[0][0]+b1PW ||checkWord == users[1][0]+b2PW ||checkWord == users[2][0]+b3PW){
        button2State = 1 - button2State;
        String json = sendSystemState();
        request->send(200, "application/json", json);
        Serial.println("Button 2 toggled successfully");
      } else {
        request->send(400, "text/plain", "Invalid check_word");
      }
    } else {
      request->send(400, "text/plain", "Missing check_word parameter");
    }
  });

  bool sseActive = false;

  server.on("/sse", HTTP_GET, [&sseActive](AsyncWebServerRequest *request){
    request->send(200, "text/event-stream", "");
    Serial.println("SSE Connection established");

    sseActive = true;

    request->onDisconnect([request, &sseActive](){
      Serial.println("Client disconnected from SSE");
      sseActive = false;
    });

    while (sseActive) {
      String jsonData = sendSystemState();
      Serial.println("Sending SSE update");
      sendSSE(request, jsonData);
      delay(1000);
    }
  });

  server.on("/button3", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("check_word")) {
      String checkWord = request->getParam("check_word")->value();

      if (checkWord == users[0][0]+b1PW ||checkWord == users[1][0]+b2PW ||checkWord == users[2][0]+b3PW) {
        button3State = 1 - button3State;
        String json = sendSystemState();
        request->send(200, "application/json", json);
        Serial.println("Button 3 toggled successfully");
      } else {
        request->send(400, "text/plain", "Invalid check_word");
      }
    } else {
      request->send(400, "text/plain", "Missing check_word parameter");
    }
  });

  server.on("/button4", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("check_word")) {
      String checkWord = request->getParam("check_word")->value();

      if (checkWord == users[0][0]+b1PW ||checkWord == users[1][0]+b2PW ||checkWord == users[2][0]+b3PW) {
        button4State = 1 - button4State;
        String json = sendSystemState();
        request->send(200, "application/json", json);
        Serial.println("Button 4 toggled successfully");
      } else {
        request->send(400, "text/plain", "Invalid check_word");
      }
    } else {
      request->send(400, "text/plain", "Missing check_word parameter");
    }
  });

  //############# ARSLAN CODE SERVER LOGIC #########

    server.on("/controlGPIO", HTTP_POST, [](AsyncWebServerRequest* request) {
    Serial.println("command run");
    if (request->hasParam("button") && request->hasParam("value")) {
      Serial.println("Condition Run");
      int button = request->getParam("button")->value().toInt();
      int value = request->getParam("value")->value().toInt();
      Serial.println((String)button);
      Serial.println((String)value);

      if(button == electricPulse_one){
        pulseOneFlag =true;
      }
       if(button == electricPulse_two){
        pulseTwoFlag =true;
      }

      // Check if the button is a valid GPIO and the value is either 0 or 1
      if (isValidGPIO(button) && (value == 0 || value == 1) && canChangeState(button)) {
        digitalWrite(button, value);
        lastChangeTime[getLedIndex(button)] = millis(); // Update last change time

        String msg = "GPIO " + (String)button + " is set to " + (String)value;

        // Create a JSON response
        DynamicJsonDocument doc(100);
        doc["success"] = true;
        doc["message"] = msg;
        String jsonResponse;
        Serial.println(jsonResponse);
        serializeJson(doc, jsonResponse);
        request->send(200, "application/json", jsonResponse);
      } else {
        // Create a JSON response for invalid parameters or attempt to change state too soon
        DynamicJsonDocument doc(100);
        doc["success"] = false;
        doc["message"] = "Invalid parameters or state change too soon";
        String jsonResponse;
        Serial.println(jsonResponse);
        serializeJson(doc, jsonResponse);

        request->send(400, "application/json", jsonResponse);
      }
    } else {
      // Create a JSON response for missing parameters
      Serial.println("Missing parms");
      DynamicJsonDocument doc(100);
      doc["success"] = false;
      doc["message"] = "Missing parameters";
      String jsonResponse;
      Serial.println(jsonResponse);
      serializeJson(doc, jsonResponse);

      request->send(400, "application/json", jsonResponse);
    }
  });

//############################## LOGIC END ############################33
  server.begin();
}

void loop() {

  if(pulseOneFlag == true){
    digitalWrite(electricPulse_one,HIGH);
    delay(3000);
    digitalWrite(electricPulse_one,LOW);
     pulseOneFlag=false;
  }

   if(pulseTwoFlag == true){
    digitalWrite(electricPulse_two,HIGH);
    delay(3000);
    digitalWrite(electricPulse_two,LOW);
    pulseTwoFlag=false;
  }
  
}

bool parseUserData(const String& data, String& username, String& password) {
  int separatorIndex = data.indexOf(':');
  if (separatorIndex != -1) {
    username = data.substring(0, separatorIndex);
    password = data.substring(separatorIndex + 1);
    return true;
  } else {
    return false;
  }
}

bool isValidUser(const char* username, const char* password) {
  for (size_t i = 0; i < sizeof(users) / sizeof(users[0]); ++i) {
    if (strcmp(username, users[i][0]) == 0 && strcmp(password, users[i][1]) == 0) {
      return true;
    }
  }
  return false;
}

String sendSystemState() {
  doc.clear();
  DynamicJsonDocument doc(100);
  // doc["button1"] = button1State;
  // doc["button2"] = button2State;
  // doc["button3"] = button3State;
  // doc["button4"] = button4State;

  doc["button1"] = digitalRead(button1);
  doc["button2"] = digitalRead(button2);
  doc["button3"] =digitalRead(button3);
  doc["button4"] = digitalRead(button4);
  String jsonString;
  serializeJson(doc, jsonString);
  return jsonString;
}



int creatUser() {
  randomSeed(analogRead(0));
  int randomNumber = random(999999);
  return randomNumber;
}

bool checkUsers(char) {
}

void sendSSE(AsyncWebServerRequest *request, const String &data) {
  request->send(200, "text/event-stream", "hello\n");
}


