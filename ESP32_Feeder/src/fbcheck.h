#include <Arduino.h>
 
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"


// Insert Firebase project API Key
#define API_KEY "AIzaSyDqMr_afNmEBaQND2FhqdAL1kOji1-_a-8"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://petfeeder-78863-default-rtdb.asia-southeast1.firebasedatabase.app/" 
#define light_PIN 2
//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int intValue;
float floatValue;
bool signupOK = false;

void setupfirebase() {
pinMode(light_PIN,OUTPUT);
  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void readfbStatus() {

    if (Firebase.RTDB.getString(&fbdo, "/light_status")) {
      if (fbdo.dataType() == "string") {
        String Value = fbdo.stringData();
        Serial.println(Value);
        if(Value.indexOf("on")>-1){
digitalWrite(light_PIN,HIGH);
Serial.println("light is on ");
        }
        else if(Value.indexOf("off") > -1){
digitalWrite(light_PIN,LOW);
Serial.println("light is off ");
        }
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }  
  
  }
