
#include <WiFi.h> // wifi lib
#include <AsyncTCP.h> // TCP lib
#include <ESPAsyncWebServer.h> // webserver lib
#include <SPIFFS.h> //spiff lib

const char *ssid = "Islo AI"; 
const char *password = "gamican3506";

#define LED_F1 23
#define LED_F2 22
#define LED_E1 19
#define LED_E2 18 
#define LED_C 21
#define LED_M 2

int LED_F1status=0; 
int LED_F2status=0;
int LED_BMstatus = 0;
int LED_BRstatus = 0;


int SW_BMON_STATUS=0;
int BRtimer=0;
int BMtimer=0;
String title2status="Title2 - ON";
String title3status="Title3 - ON";
int LED_E1status=0;
int LED_E2status=0;
unsigned long sw1millis=0;
unsigned long sw2millis=0;
boolean sw1Flag=false;
boolean sw2Flag=false;
int LED_Cstatus=0;
int LED_Mstatus=0;
#define FLASH_INTERVAL 1000
bool ledState = false;
unsigned long previousTimestampfor_ledM = 0;
bool ledmflag = false;

// Variables to track the states and timing
bool slider1State = false;
bool slider2State = false;
bool slider3State = false;
bool slideroneFlag=false;
bool slidertwoFlag=false;
bool sliderthreeFlag=false;
unsigned long previousTimestamp;

unsigned long toggleOneMillis=0;

// VARIABLES FOR SLIDER 3
int slider3count=0;
int  e1flagcontneous=0; // to control contineous beep
int  e2flagcontneous=0;


// PINS DEFINATION
#define BUZZER_PIN 15
#define SW_F1 32
#define SW_F2 33 
#define SW_BM 25
#define SW_BR 26
#define SW_M 27


const char *input_parameter1 = "output"; // this will be used to geting the status from html page like key and value
const char *input_parameter2 = "state";
unsigned long premillis = 0;  

// Creating a AsyncWebServer object
AsyncWebServer server(80);


String outputState(int output)   // this is the state of slider
{
  if (digitalRead(output))
  {
    return "";
  }
  else
  {
    return "";
  }
}

// Replaces placeholder with button section in your web page
String processor(const String &var)  // slider code is this all other leds code is in index page only slider code is here
{
  // Serial.println(var);
  if (var == "BUTTONPLACEHOLDER")   // we can get this slider in html page by using only this %BUTTONPLACEHOLDER% in index page
  {
    String buttons = "";
 
    return buttons;
  }
  return String();
}

String sendtoServer()   // json string that will send valus of leds and buttons  after every .5 sec
{
 String jsonString = "{\"ledf1\":\"" + (String)LED_F1status + "\",\"ledf2\":\"" + (String)LED_F2status + "\",\"title2\":\"" + (String)title2status + "\",\"title3\":\"" + (String)title3status + "\",\"lede1\":\"" + (String)LED_E1status+ "\",\"lede2\":\"" + (String)LED_E2status + "\",\"round1\":\"" + (String)LED_BMstatus + "\",\"round2\":\"" + (String)LED_BRstatus + "\",\"ledc\":\"" + (String)LED_Cstatus + "\",\"ledm\":\"" + (String)LED_Mstatus + "\"}";
 // Serial.println(jsonString);
  return jsonString;
}


// Function to handle Slider 1 logic
void handleSlider1() {
  
  if(slideroneFlag==true){
  // Check if any of the specified LEDs are turned on
  if ((digitalRead(LED_F1) == HIGH) || (digitalRead(LED_F2) == HIGH) || (digitalRead(LED_C)== HIGH)) {
    Serial.println("SLIDER 1 IS ON");
    digitalWrite(BUZZER_PIN, HIGH);
    if(millis() - toggleOneMillis > 4000){
     digitalWrite(BUZZER_PIN, LOW);
       Serial.println("4 sec completed");
    
  slider1State=false;
    }
  } 
  }
}

// Function to handle Slider 2 logic
void handleSlider2() {
   if(slidertwoFlag==true){
   // Define the sound pattern (ON and OFF durations in milliseconds)
  int soundPattern[] = {500, 500, 500, 1000, 500, 500, 500, 1000, 500, 500, 500, 0};
  
  // Loop to play the sound pattern 6 times
  for (int repeat = 0; repeat < 1; repeat++) {
    for (int i = 0; i < sizeof(soundPattern) / sizeof(int); i++) {
      if (soundPattern[i] > 0) {
        // Turn ON the buzzer
        if( soundPattern [i]  == 1000){
          delay(1000);
        }else{
        digitalWrite(BUZZER_PIN, HIGH);
        delay(500);
         digitalWrite(BUZZER_PIN, LOW);
         delay(500);}

      } else {
        // Turn OFF the buzzer
        digitalWrite(BUZZER_PIN, LOW);
        delay(500); // Delay at the end of each repetition
      }
    }
    
  }
  slider2State=false;
  e1flagcontneous++;
  e2flagcontneous++;
   }
}



void setup() {
  Serial.begin(115200);
Serial.println("CODE STARTED");
  pinMode(LED_F1, OUTPUT);
  pinMode(LED_F2, OUTPUT);
  pinMode(LED_E1, OUTPUT);
  pinMode(LED_E2, OUTPUT);
  pinMode(LED_C, OUTPUT);
  pinMode(LED_M, OUTPUT);

  digitalWrite(LED_F1, LOW);
  digitalWrite(LED_F2, LOW);
  digitalWrite(LED_E1, LOW);
  digitalWrite(LED_E2, LOW);
  digitalWrite(LED_C, LOW);
  digitalWrite(LED_M, LOW);
  
  pinMode(BUZZER_PIN, OUTPUT);

  
  pinMode(SW_F1, INPUT);
  pinMode(SW_F2, INPUT);
  pinMode(SW_BM, INPUT);
  pinMode(SW_BR, INPUT);
  pinMode(SW_M, INPUT);

// SPIFF BEGIN
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount SPIFFS");
    return;
  }

  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");
// CONNECTING WIFI
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
  delay(10000);
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/code.html", "text/html");
  });

// ROUTE ADDRESS FOR GETTING STATUS OF LEDS AND SEND JSON DATA TO THE WEBSERVER
  server.on("/getledstatus", HTTP_GET, [](AsyncWebServerRequest *request) // this request will used to update response the json string from esp32 to server
            {
String json = sendtoServer();
request->send(200, "application/json", json);
json = String(); });

  // Send a GET request to <ESP_IP>/DATA?output=<inputMessage1>&state=<inputMessage2>
  server.on("/DATA", HTTP_GET, [](AsyncWebServerRequest *request)  // all data is cooming on this address
            {
String inputMessage1;
String inputMessage2;
// GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
if (request->hasParam(input_parameter1) && request->hasParam(input_parameter2)) {
inputMessage1 = request->getParam(input_parameter1)->value();
inputMessage2 = request->getParam(input_parameter2)->value();

// CONDITION FOR TOGGLE ONE BUTTON
 if((inputMessage1.indexOf("toggleone")>-1) && (inputMessage2.indexOf("ON")>-1)){
  toggleOneMillis=millis();
  slider1State=true;
  slideroneFlag=true;
 }
  if((inputMessage1.indexOf("toggleone")>-1) && (inputMessage2.indexOf("OFF")>-1)){
 
  slideroneFlag=false;
 }

// CONTITION FOR TOGGLE TWO BUTTON
  if((inputMessage1.indexOf("toggletwo")>-1) && (inputMessage2.indexOf("ON")>-1)){
slidertwoFlag=true;
   slider2State=true; 
 }

  if((inputMessage1.indexOf("toggletwo")>-1) && (inputMessage2.indexOf("OFF")>-1)){

   slidertwoFlag=false; 
 }

// CONDITION FOR TOGGLE THREE BUTTON
   if((inputMessage1.indexOf("togglethree")>-1) && (inputMessage2.indexOf("ON")>-1)){
sliderthreeFlag=true;
slider3State=true;
  
 }

    if((inputMessage1.indexOf("togglethree")>-1) && (inputMessage2.indexOf("OFF")>-1)){
sliderthreeFlag=false;
  
 }

// CONTION FOR F1
 if(inputMessage1.indexOf("f1")>-1){
  if(LED_F1status == 0){
  LED_F1status=1;
  digitalWrite(LED_F1,HIGH);
    toggleOneMillis=millis();
  slider1State=true;}
  else{
     LED_F1status=0;
  digitalWrite(LED_F1,LOW);
  }
 }

// CONDITION FOR F2
  if(inputMessage1.indexOf("f2")>-1){
  if(LED_F2status == 0){
  LED_F2status=1;
  digitalWrite(LED_F2,HIGH);
    toggleOneMillis=millis();
  slider1State=true;}
  else{
     LED_F2status=0;
  digitalWrite(LED_F2,LOW);
  }
 }

// CONDITION FOR E1
   if(inputMessage1.indexOf("e1")>-1){
  if(LED_E1status == 0){
  LED_E1status=1;
  digitalWrite(LED_E1,HIGH);
    slider2State=true;}
  else{
     LED_E1status=0;
  digitalWrite(LED_E1,LOW);
  }
 }

//CONDITION FOR E2 BUTTON
   if(inputMessage1.indexOf("e2")>-1){
  if(LED_E2status == 0){
  LED_E2status=1;
  digitalWrite(LED_E2,HIGH);
    slider2State=true;}
  else{
     LED_E2status=0;
  digitalWrite(LED_E2,LOW);
  }
 }

//CONDITION FOR C BUTTON 
    if(inputMessage1.indexOf("c")>-1){
  if(LED_Cstatus == 0){
  LED_Cstatus=1;
  digitalWrite(LED_C,HIGH);
    toggleOneMillis=millis();
  slider1State=true;}
  else{
     LED_Cstatus=0;
  digitalWrite(LED_C,LOW);
  }
 }

// CONDITON FOR M BUTTON 
     if(inputMessage1.indexOf("m")>-1){
      if(ledmflag == false){
 ledmflag=true;
 slider3State = true;

      }
      else{ledmflag=false;}
 }
 
 
}
// JUST PRINTING DATA WHAT WE GET
Serial.print("KEY: ");
Serial.print(inputMessage1);
Serial.print(" - DATA: ");
Serial.println(inputMessage2);
request->send(200, "text/plain", "OK"); });

  // Start server
  server.begin();
}

void loop()
{
 // CONDITON FOR CHECKING SW_BM STATUS
  if ((digitalRead(SW_BM) == HIGH) && (SW_BMON_STATUS == 0)) {
    digitalWrite(LED_F1, LOW);
    LED_F1status=0;
    digitalWrite(LED_F2, LOW);
    LED_F2status=0;
    LED_BMstatus=1;
    SW_BMON_STATUS++;
  }

// // CONDITON FOR CHECKING SW_Br STATUS
if((digitalRead(SW_BR) == HIGH) && (BMtimer==0)){
   digitalWrite(LED_F1, LOW);
    LED_F1status=0;
    digitalWrite(LED_F2, LOW);
    LED_F2status=0;
    LED_BRstatus=1;
  BMtimer++;
}


// CHANGING STATUS
if (digitalRead(SW_F1) == HIGH) {
  Serial.println("SWF2_LOW");
if(e1flagcontneous == 0){
sw1Flag=true;
}
//Serial.println("F1 HIGH");
}
else{
 title2status="Title2 – ON"; 
 sw1Flag=false;
}

if (digitalRead(SW_F2) == HIGH) {
    Serial.println("BUTTON IS CLICKED ");
title3status="Title3 – OFF";
if(e2flagcontneous == 0){
   Serial.println("e2FLAG "+(String)e2flagcontneous);
sw2Flag=true;
}
}
else{
 title3status="Title3 – ON"; 
 sw2Flag=false;
}

// THIS LOGIC WILL CREATE LED E1 HIGH WHEN SW1 IS HIGH
if(sw1Flag == true){
if((millis()- sw1millis > 5000)  && (e1flagcontneous == 0)){
  digitalWrite(LED_E1,HIGH);
  slider2State=true;
  LED_E1status=1;
}}
else{
  sw1millis=millis();
  e1flagcontneous=0;
}

// THIS LOGIC WILL CREATE LED E2 HIGH WHEN SW2 IS HIGH
if(sw2Flag == true){
  if ((millis()- sw2millis > 5000) && (e2flagcontneous == 0)){
  digitalWrite(LED_E2,HIGH);
  slider2State=true;
  LED_E2status=1;
}}
else{
  sw2millis=millis();
    e2flagcontneous=0;
}

// THIS FUNCTION IS FOR BUTTON M LOGIC WE ALSO DEAL SLIDER 3 LOGIC HERE
if (ledmflag) {
    // Get the current timestamp
    unsigned long currentTimestamp = millis();

    // Check if the specified interval has elapsed
    if (currentTimestamp - previousTimestampfor_ledM >= FLASH_INTERVAL) {
      // Update the previous timestamp
      previousTimestampfor_ledM = currentTimestamp;
if((slider3count == 0) && (sliderthreeFlag == true)){
  digitalWrite(BUZZER_PIN,HIGH);
}
 if((slider3count == 1) && (sliderthreeFlag == true)){
  digitalWrite(BUZZER_PIN,LOW);
}
 if((slider3count == 2) && (sliderthreeFlag == true)){
  digitalWrite(BUZZER_PIN,HIGH);
}
 if((slider3count == 3) && (sliderthreeFlag == true)){
  digitalWrite(BUZZER_PIN,LOW);
}
 if((slider3count == 4) && (sliderthreeFlag == true)){
  digitalWrite(BUZZER_PIN,HIGH);
}
 if((slider3count > 4) && (sliderthreeFlag == true)){
  digitalWrite(BUZZER_PIN,LOW);
}
slider3count++;
      // Toggle the LED state
      ledState = !ledState;

     
      digitalWrite(LED_M, ledState);
      if(ledState){
        LED_Mstatus=1;
        digitalWrite(LED_M,HIGH);
    
      }else{
        LED_Mstatus=0;
        digitalWrite(LED_M,LOW);
       
      }
    }
  }
  else{
     LED_Mstatus=0;
        digitalWrite(LED_M,LOW);
        slider3count=0;
  }

  if(digitalRead(SW_M) == HIGH){
    ledmflag=false;
    LED_Mstatus=0;
     digitalWrite(LED_M,LOW);
  }

// DEAL SLIDER ONE LOGIC HERE
if(slider1State ==true){
  handleSlider1();
}

// DEAL SLIDER TWO LOGIC HERE
if(slider2State == true){
    if((digitalRead(LED_E1) == HIGH) || (digitalRead(LED_E2) == HIGH) ){
  handleSlider2();
    }
}


}
  