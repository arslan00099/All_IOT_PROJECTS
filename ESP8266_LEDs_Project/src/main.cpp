#include "index.php" // this will include html ,css and javascript code into main code

const char *ssid = "Hack";
const char *password = "";
// const char *ssid = "TEST1234";
// const char *password = "";

#define LED_F1 D3
#define LED_F2 D8
#define LED_E1 9 //SD2
#define LED_E2 10 //SD3
#define LED_C D6
#define LED_M D4

int LED_F1status=0; 
int LED_F2status=0;
int firsttime=0;
int LED_BMstatus = 0;
int LED_BRstatus = 0;
int BMtimer=0;
int BRtimer=0;
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
// Define the time intervals for different patterns in milliseconds
#define BUZZER_ON_OFF_INTERVAL 500
#define BUZZER_SEQUENCE_INTERVAL 1000
#define BUZZER_SEQUENCE_REPEAT 6
#define BUZZER_SEQUENCE_PAUSE 500
// Variables to track the states and timing
bool slider1State = false;
bool slider2State = false;
bool slider3State = false;
bool buzzerState = false;
unsigned long previousTimestamp;
unsigned int buzzerSequenceCounter;




#define BUZZER_PIN D7

#define SW_F1 5 //D1
#define SW_F2 D0 
#define SW_BM D5
#define SW_BR D6
#define SW_M D8

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
  Serial.println(jsonString);
  return jsonString;
}


// Function to handle Slider 1 logic
void handleSlider1() {
  // Check if any of the specified LEDs are turned on
  if (digitalRead(LED_F1) || digitalRead(LED_F2) || digitalRead(LED_C)) {
    // Turn on the buzzer
    digitalWrite(BUZZER_PIN, HIGH);
    delay(4000);
     digitalWrite(BUZZER_PIN, LOW);
    buzzerState = true;
  } else {
    // Turn off the buzzer if it was previously on
    if (buzzerState) {
      digitalWrite(BUZZER_PIN, LOW);
      buzzerState = false;
    }
  }
}

// Function to handle Slider 2 logic
void handleSlider2() {
  // Check if any of the specified LEDs are turned on
  if (digitalRead(LED_E1) || digitalRead(LED_E2)) {
    // Calculate the time difference for the buzzer sequence
    unsigned long currentTimestamp = millis();
    unsigned long timeDiff = currentTimestamp - previousTimestamp;

    // Perform the buzzer sequence based on the time difference
    if (timeDiff >= BUZZER_ON_OFF_INTERVAL) {
      previousTimestamp = currentTimestamp;

      if (buzzerSequenceCounter < BUZZER_SEQUENCE_REPEAT * 2) {
        // Determine whether to turn the buzzer on or off
        if (buzzerSequenceCounter % 2 == 0) {
          digitalWrite(BUZZER_PIN, HIGH);
          buzzerState = true;
        } else {
          digitalWrite(BUZZER_PIN, LOW);
          buzzerState = false;
        }

        // Increment the sequence counter
        buzzerSequenceCounter++;
      } else if (timeDiff >= BUZZER_SEQUENCE_PAUSE) {
        // Reset the sequence counter after the pause
        buzzerSequenceCounter = 0;
      }
    }
  } else {
    // Turn off the buzzer if it was previously on
    if (buzzerState) {
      digitalWrite(BUZZER_PIN, LOW);
      buzzerState = false;
    }
  }
}

// Function to handle Slider 3 logic
void handleSlider3() {
  // Check if LED_M is turned on
  if (digitalRead(LED_M)) {
    // Calculate the time difference for the buzzer sequence
    unsigned long currentTimestamp = millis();
    unsigned long timeDiff = currentTimestamp - previousTimestamp;

    // Perform the buzzer sequence based on the time difference
    if (timeDiff >= BUZZER_ON_OFF_INTERVAL) {
      previousTimestamp = currentTimestamp;

      if (buzzerSequenceCounter < 6) {
        // Determine whether to turn the buzzer on or off
        if (buzzerSequenceCounter % 2 == 0) {
          digitalWrite(BUZZER_PIN, HIGH);
          buzzerState = true;
        } else {
          digitalWrite(BUZZER_PIN, LOW);
          buzzerState = false;
        }

        // Increment the sequence counter
        buzzerSequenceCounter++;
      } else if (timeDiff >= BUZZER_SEQUENCE_PAUSE) {
        // Reset the sequence counter after the pause
        buzzerSequenceCounter = 0;
      }
    }
  } else {
    // Turn off the buzzer if it was previously on
    if (buzzerState) {
      digitalWrite(BUZZER_PIN, LOW);
      buzzerState = false;
    }
  }
}



void setup()
{   Serial.begin(115200);
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

  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html, processor); });

  server.on("/getledstatus", HTTP_GET, [](AsyncWebServerRequest *request) // this request will used to update response the json string from esp32 to server
            {
String json = sendtoServer();
request->send(200, "application/json", json);
json = String(); });

  // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/DATA", HTTP_GET, [](AsyncWebServerRequest *request)  // this will be used for slider values
            {
String inputMessage1;
String inputMessage2;
// GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
if (request->hasParam(input_parameter1) && request->hasParam(input_parameter2)) {
inputMessage1 = request->getParam(input_parameter1)->value();
inputMessage2 = request->getParam(input_parameter2)->value();
 if((inputMessage1.indexOf("toggleone")>-1) && (inputMessage2.indexOf("ON")>-1)){
 digitalWrite(LED_F1,HIGH);
 LED_F1status=1;
  digitalWrite(LED_F2,HIGH);
 LED_F2status=1;
  digitalWrite(LED_C,HIGH);
 LED_Cstatus=1;
   previousTimestamp = millis();
  buzzerSequenceCounter = 0;
  slider1State=true;
 }
 else if((inputMessage1.indexOf("toggleone")>-1) && (inputMessage2.indexOf("OFF")>-1)){
 digitalWrite(LED_F1,LOW);
 LED_F1status=0;
  digitalWrite(LED_F2,LOW);
 LED_F2status=0;
  digitalWrite(LED_C,LOW);
 LED_Cstatus=0;
 slider2State=false;
 }

  if((inputMessage1.indexOf("toggletwo")>-1) && (inputMessage2.indexOf("ON")>-1)){
 digitalWrite(LED_E1,HIGH);
 LED_E1status=1;
  digitalWrite(LED_E2,HIGH);
 LED_E2status=1;
   previousTimestamp = millis();
  buzzerSequenceCounter = 0;
  slider2State=true;
 }
 else if((inputMessage1.indexOf("toggletwo")>-1) && (inputMessage2.indexOf("OFF")>-1)){
 digitalWrite(LED_E1,LOW);
 LED_E1status=0;
   digitalWrite(LED_E2,LOW);
 LED_E2status=0;
 slider2State=false;
 }

   if((inputMessage1.indexOf("togglethree")>-1) && (inputMessage2.indexOf("ON")>-1)){
ledmflag=true;
  previousTimestamp = millis();
  buzzerSequenceCounter = 0;
slider3State=true;
 }else{slider3State=false;}
 
 
}
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
  // Serial.println("##############################");
  // Serial.println(digitalRead(SW_F1));
  // Serial.println(digitalRead(SW_F2));
  // Serial.println(digitalRead(SW_BM));
  // Serial.println(digitalRead(SW_BR));
  //  Serial.println(digitalRead(SW_M));
  // Serial.println("##############################");
  //Serial.println();
  if ((digitalRead(SW_BM) == HIGH || digitalRead(SW_BR) == HIGH) && (firsttime == 0)) {
    digitalWrite(LED_F1, LOW);
    digitalWrite(LED_F2, LOW);
    firsttime++;
  }

if((digitalRead(SW_BM) == HIGH && BMtimer==0)){
  LED_BMstatus=1;
  BMtimer++;
}

if((digitalRead(SW_BR) == HIGH && BRtimer==0)){
  LED_BRstatus=1;
  BRtimer++;
}

if (digitalRead(SW_F1) == HIGH) {
title2status="Title2 – OFF";
sw1Flag=true;
}

if (digitalRead(SW_F2) == HIGH) {
title3status="Title3 – OFF";
sw2Flag=true;
}

if(sw1Flag == true){
if(millis()- sw1millis > 5000){
  digitalWrite(LED_E1,HIGH);
  LED_E1status=1;
}}
else{
  sw1millis=millis();
}

if(sw2Flag == true) {
  if (millis()- sw2millis > 5000){
  digitalWrite(LED_E2,HIGH);
  LED_E2status=1;
}}
else{
  sw2millis=millis();
}


if (ledmflag) {
    // Get the current timestamp
    unsigned long currentTimestamp = millis();

    // Check if the specified interval has elapsed
    if (currentTimestamp - previousTimestampfor_ledM >= FLASH_INTERVAL) {
      // Update the previous timestamp
      previousTimestampfor_ledM = currentTimestamp;

      // Toggle the LED state
      ledState = !ledState;

      // Set the LED pin to the new state
      digitalWrite(LED_M, ledState);
      if(ledState){
        LED_Mstatus=1;
      }else{
        LED_Mstatus=0;
      }
    }
  }

  if(digitalRead(SW_M) == HIGH){
    ledmflag=false;
    LED_Mstatus=0;
  }

    // Check the states of the sliders
  if (slider1State) {
    handleSlider1();
  }

  if (slider2State) {
    handleSlider2();
  }

  if (slider3State) {
    handleSlider3();
  }

}
  