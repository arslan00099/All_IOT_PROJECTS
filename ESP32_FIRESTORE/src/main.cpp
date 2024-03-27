
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include <ESP32Servo.h>
#include <ESP32Time.h>
ESP32Time rtc(3600);


#define LED_PIN 2 // Set the LED pin as a constant variable
#define servo_PIN 4
unsigned long previousMillis = 0; // Variable to store the previous time
const long interval = 800; // Interval of blinking in milliseconds
String device_id="123456";
String devie_pw="myPw";
Servo myservo;
int pos = 0;

const char* ntpServer = "europe.pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
unsigned long epochTime;


#define WIFI_SSID "RIGJAZZ"
#define WIFI_PASSWORD "Kashmiri786"

#define API_KEY "AIzaSyD0N2fhaSWmJ86yjQQimX_tm48F6Yw-H4A"

#define DATABASE_URL "https://esp32-hiot-01-default-rtdb.europe-west1.firebasedatabase.app/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

#define USER_EMAIL "esp@gmail.com"
#define USER_PASSWORD "#Esp00998"


FirebaseData stream;
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

int count = 0;

volatile bool dataChanged = false;


void blinkLED(){
    unsigned long currentMillis = millis(); // Get the current time

  if (currentMillis - previousMillis >= interval) { // If the interval has elapsed
    previousMillis = currentMillis; // Reset the previous time
    digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // Toggle the LED state
  }


}

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}

void getntp() {

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  epochTime = getTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);
  rtc.setTime(epochTime);
  

}

void streamCallback(FirebaseStream data)
{
  Serial.printf("sream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
                data.streamPath().c_str(),
                data.dataPath().c_str(),
                data.dataType().c_str(),
                data.eventType().c_str());
  printResult(data); // see addons/RTDBHelper.h
  // Serial.println("PATH : "+(String)data.dataPath());
  // Serial.println("DATA : "+(String)data.payload());
  String path=data.dataPath();
  String payloaddata=data.payload();
 
  Serial.println(path);


  Serial.printf("Received stream payload size: %d (Max. %d)\n\n", data.payloadLength(), data.maxPayloadLength());
  if(data.payloadLength()>100){
      StaticJsonDocument<400> doc;
       DeserializationError error = deserializeJson(doc, data.payload());

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
   String led_status = doc["led_on"];
   int servo_Angle=doc["servo_value"];
   Serial.println(led_status);
   Serial.println((String)servo_Angle);
   	myservo.write(servo_Angle);
    if(led_status.indexOf("true")>-1){
      digitalWrite(LED_PIN,HIGH);
    }
    else{
       digitalWrite(LED_PIN,LOW);
    }
  }
  else{
    if(path.indexOf("led_on")>-1){
      if(payloaddata.indexOf("true")>-1){
        digitalWrite(LED_PIN,HIGH);
      }
      else {
       digitalWrite(LED_PIN,LOW); 
      }
    }
    if(path.indexOf("servo_value")>-1){
      int angle = payloaddata.toInt();
      myservo.write(angle);
    }
   
  }

  dataChanged = true;
}

void streamTimeoutCallback(bool timeout)
{
  if (timeout)
    Serial.println("stream timed out, resuming...\n");
    blinkLED();

  if (!stream.httpConnected())
    Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
}

void settingUpFirebase(){
    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);


  config.api_key = API_KEY;


  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;


  config.database_url = DATABASE_URL;

config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
Firebase.begin(&config, &auth);
Firebase.reconnectWiFi(true);

if (!Firebase.RTDB.beginStream(&stream, "/"+(String)device_id))
    Serial.printf("sream begin error, %s\n\n", stream.errorReason().c_str());

  Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);
}



void setup()
{

  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
myservo.attach(servo_PIN, 1000, 2000); // attaches the servo on pin 18 to the servo object
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    blinkLED();
    delay(300);

  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  while (epochTime < 1){
  getntp();
}


}

void loop()
{
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    count++;
    FirebaseJson json;
    json.add("epouch", rtc.getEpoch());
    
    Serial.printf("Set json... %s\n\n", Firebase.RTDB.setJSON(&fbdo, "/"+device_id+"/timestamp", &json) ? "ok" : fbdo.errorReason().c_str());
  }

if(Firebase.ready()){
 
  if (dataChanged)
  {
    dataChanged = false;
  }
}
else {
settingUpFirebase();
 blinkLED();
}
}
