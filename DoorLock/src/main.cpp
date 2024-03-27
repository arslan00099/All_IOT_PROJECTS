// Importing required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "Servo.h"
#include <Keypad.h>



// Define the Keypad pins
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {14, 15, 27, 26};
byte colPins[COLS] = {18, 19, 4, 23};

const char* LOCK_PASSWORD = "0000";
const char* UNLOCK_PASSWORD = "1111";

// Create an instance of the Keypad class
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Replace with your network credentials
const char* ssid = "RIGJAZZ";
const char* password = "Kashmiri786";


// Setting LED GPIO

const int ledPinG = 12;
const int ledPinR = 13;

// Setting Servo PIN

#define servoPin 2
Servo servo1;


//AsyncWebServer object on port 80
AsyncWebServer server(80);

// Flag to indicate if the door is unlocked
bool doorUnlocked = false;
bool doorLocked = false;
unsigned long unlockTime = 0;

void setup() {

  // Serial port for debugging purposes
  Serial.begin(115200);
  
  pinMode(ledPinG, OUTPUT); // Set the LED pin as an output
  pinMode(ledPinR, OUTPUT); // Set the LED pin as an output
  
  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to WiFi Succesfully");
  
  // Print ESP32 Local IP Address
  Serial.println("Copy the following IP Adress and paste it into your web browser and visit the website");
  Serial.println(WiFi.localIP());

  servo1.attach(servoPin);
  
  // For root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    
    request->send(SPIFFS, "/index.html", "text/html");
  });

  //To load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    
    request->send(SPIFFS, "/style.css", "text/css");
    
  });

  // Route to set GPIO to HIGH
  server.on("/180", HTTP_GET, [](AsyncWebServerRequest * request) {
    delay(1500);
    servo1.write(180); 
    Serial.println(request->url());
    
    doorLocked = true;
    unlockTime = 0;
    digitalWrite(ledPinR, HIGH); // Turn the LED on
    digitalWrite(ledPinG, LOW); // Turn the LED off
  });

  // Route to set GPIO to LOW
  server.on("/0", HTTP_GET, [](AsyncWebServerRequest * request) {
 
    delay(1500);
    
    digitalWrite(ledPinG, HIGH); // Turn the LED on
    digitalWrite(ledPinR, LOW); // Turn the LED off
    
    servo1.write(0);
    
    Serial.println(request->url());
    
    
    unlockTime = millis();
    doorLocked = false;
    digitalWrite(ledPinG, HIGH); // Turn the LED on
    digitalWrite(ledPinR, LOW); // Turn the LED off

  });


  // Start server
  server.begin();
  

}
void loop() {
  // Check if a key on the keypad has been pressed
  char key = keypad.getKey();
  
  if (key != NO_KEY) {
    static String password = "";
    
    if (key == '#') {
      
      // End of password, check if it's the lock or unlock password
     
      if (password == LOCK_PASSWORD) {
        servo1.write(180);
        doorLocked = true;
        //Door lock time reset
        unlockTime = 0;
        digitalWrite(ledPinR, HIGH); // Turn the LED on
        digitalWrite(ledPinG, LOW); // Turn the LED off
        
      } else if (password == UNLOCK_PASSWORD) {
        servo1.write(0);
        doorLocked = false;
        unlockTime = millis();
        
        digitalWrite(ledPinG, HIGH); // Turn the LED on
        digitalWrite(ledPinR, LOW); // Turn the LED off
      }
      
else {
  Serial.println("Wrong Password, please try again");
  }
      password = "";
    } else {
      // Add key to password string
      password += key;
      Serial.print(key);
    }
    
  }
    
  // Check if 10 seconds have passed since unlocking the door
  
  if(!doorLocked && unlockTime > 0 && millis() - unlockTime >= 10000) {
    servo1.write(180);
    doorLocked = true;
    unlockTime = 0;
    digitalWrite(ledPinR, HIGH); // Turn the LED on
    digitalWrite(ledPinG, LOW); // Turn the LED off
  }
}
