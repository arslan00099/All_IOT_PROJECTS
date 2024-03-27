#include "index.h" // this will include html ,css and javascript code into main code
int count = 0; //inlizing the variables
boolean counterflag = false;  // falg for checking conditions
unsigned premillistest = 0;   // this will be used for slider on function
boolean testflag = false;    // this will be used to check the slider postion in loop function

// ################### FOR DEBOUNCE msSwitch
const unsigned long debounceDelay = 50; // Debounce delay in milliseconds

int msSwitchState = HIGH;           // Initial state of the switch
int lastSwitchState = HIGH;         // Previous state of the switch
unsigned long lastDebounceTime = 0; // Last time the switch state changed

/// #############

// ###### FOR PUSH BUTTON

int pushState = HIGH;     // Initial state of the switch
int lastpushState = HIGH; // Previous state of the switch
unsigned long pushMillis = 0;  // it will be used for push button in debounce function
boolean pushFlag = false;   //initializing flag for push button for checking condition
// ################################

// Replace with your network credentials
const char *ssid = "TEST123";
const char *password = "";

const char *input_parameter1 = "output"; // this will be used to geting the status from html page like key and value
const char *input_parameter2 = "state";
unsigned long premillis = 0;  

#define LED_A D5 //LED A pin 14
#define LED_B D6 //LED B pin 12
#define LED_C D7 //LED C pin 13
#define LED_U D3 //LED U pin 0
#define msSwitch D2 // MSWITCH pin 4
#define buttonPin D1 //PUSH BUTTON pin 5

// Creating a AsyncWebServer object
AsyncWebServer server(80);

int led_Astatus = 2; // 2 is for gray , 1 is for red and 0 is for green
int led_Bstatus = 2;
int led_Cstatus = 2;
int led_Ustatus = 2;
int led_Xstatus = 2;
int sliderstatus = 2;
boolean closetoOpenFlag = false;
boolean opentoCloseFlag = false;
boolean sliderFlag = false;

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
    buttons += "<label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"5\" " + outputState(5) + "><span class=\"slider\"></span></label>";

    return buttons;
  }
  return String();
}

String sendtoServer()   // json string that will send valus of leds and buttons  after every .5 sec
{
  String jsonString = "{\"led1\":\"" + (String)led_Astatus + "\",\"led2\":\"" + (String)led_Bstatus + "\",\"led3\":\"" + (String)led_Cstatus + "\",\"led4\":\"" + (String)led_Ustatus + "\",\"led5\":\"" + (String)led_Xstatus + "\",\"slider\":\"" + (String)sliderstatus + "\"}";
  return jsonString;
}

void mSwitchOpenToClose()   // function for switch open to close.
{
  count++;

  if (counterflag == true)
  {
    digitalWrite(LED_U, LOW);
    led_Ustatus = 2;
  }

  else
  {

    digitalWrite(LED_U, HIGH);
    led_Ustatus = 1;
  }

  if ((millis() - premillis > 4000))
  {
    digitalWrite(LED_A, HIGH);
    led_Astatus = 0;
  }

  if (millis() - premillis > 7000)
  {
    digitalWrite(LED_B, HIGH);
    led_Bstatus = 0;
  }

  if (millis() - premillis > 9000)
  {
    digitalWrite(LED_C, HIGH);
    digitalWrite(LED_U, LOW);
    led_Cstatus = 0;
    led_Ustatus = 2;
    counterflag = true;
  }
}

void mSwitchCloseToOpen()   // function for switch close to open
{
  counterflag = false;
  if (count == 0)
  {
    digitalWrite(LED_U, LOW);
    led_Ustatus = 2;
  }
  else
  {
    digitalWrite(LED_U, HIGH);
    led_Ustatus = 1;
  }

  if ((millis() - premillis > 4000))
  {
    digitalWrite(LED_A, LOW);
    led_Astatus = 2;
  }

  if (millis() - premillis > 7000)
  {
    digitalWrite(LED_B, LOW);
    led_Bstatus = 2;
  }

  if (millis() - premillis > 9000)
  {
    digitalWrite(LED_C, LOW);
    digitalWrite(LED_U, LOW);
    led_Cstatus = 2;
    led_Ustatus = 2;
    count = 0;
  }
}

void sliderOFFtoON()  // function for slider from off to on 
{
  digitalWrite(LED_U, HIGH);
  digitalWrite(LED_C, LOW);
  led_Ustatus = 1; // 1 is for on and 0 is for off
  led_Cstatus = 2;// 2 for grey
}

void deBounceFun()   // function for debounce for msswitch it is used to get only one time 1 or 0 not when ever state change for more understaning you can see any video on debouncing function 
{
  int reading = digitalRead(msSwitch);

  if (reading != lastSwitchState)
  {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != msSwitchState)
    {
      msSwitchState = reading;

      // Print the switch state
      // Serial.println(msSwitchState);

      // Add your desired actions based on the switch state
      if (msSwitchState == LOW)
      {
        premillis = millis();
        opentoCloseFlag = true;
        closetoOpenFlag = false;
        Serial.println("CLOSE TO OPEN");
      }
      else
      {
        premillis = millis();
        opentoCloseFlag = false;
        closetoOpenFlag = true;
        Serial.println("OPEN TO CLOSE");

        // Switch is released
      }
    }
  }

  lastSwitchState = reading;
}

void pushButtonDebounce()   //debounce function for push button
{
  int reading = digitalRead(buttonPin);

  if (reading != lastpushState)
  {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != pushState)
    {
      pushState = reading;

      // Add your desired actions based on the switch state
      if (pushState == LOW)
      {
        Serial.println("Released");
        led_Xstatus = 0;
        pushFlag = false;
        sliderstatus = 2;
      }
      else
      {
        Serial.println("Pressed");
        led_Xstatus = 1;
        pushMillis = millis();
        pushFlag = true;
      }
    }
  }

  lastpushState = reading;

  if ((pushFlag == true) && (millis() - pushMillis > 5000)) // this condtion check for 5 sec completed
  {
    Serial.println("5 sec completed");
    led_Xstatus = 1;
    led_Ustatus = 2;
    led_Cstatus = 0;
    led_Bstatus = 0; // added
    led_Astatus = 0; // added
    sliderstatus = 1;
    digitalWrite(LED_U, LOW);
    digitalWrite(LED_C, HIGH);
    digitalWrite(LED_B, HIGH); // added
    digitalWrite(LED_A, HIGH); // added
    sliderFlag = false;
    pushFlag = false;
  }
}

void correctoperation()   // this funcaiton will be used when we change the slider in between the ms switch functioning 
{
  int a = digitalRead(LED_A);
  if ((a == 1) && (millis() - premillistest < 4000)) 
  {
    led_Astatus = 0;
    if (millis() - premillistest > 3000)
    {
      digitalWrite(LED_B, HIGH);
      led_Bstatus = 0;
      testflag = false;
    }
  }
  if ((a == 1) && (millis() - premillistest > 4000))
  {
    led_Astatus = 0;
    if (millis() - premillistest > 7000)
    {
      digitalWrite(LED_B, HIGH);
      led_Bstatus = 0;
      testflag = false;
    }
  }
  else
  {
    if (millis() - premillistest > 4000)
    {
      digitalWrite(LED_A, HIGH);
      led_Astatus = 0;
    }
  }
}

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(LED_A, OUTPUT);
  digitalWrite(LED_A, LOW);
  pinMode(LED_B, OUTPUT);
  digitalWrite(LED_B, LOW);
  pinMode(LED_C, OUTPUT);
  digitalWrite(LED_C, LOW);
  pinMode(LED_U, OUTPUT);
  digitalWrite(LED_U, LOW);
  pinMode(msSwitch, INPUT);
  pinMode(buttonPin, INPUT);

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
  Serial.println("SWITCH : " + (String)digitalRead(msSwitch));

  int a = digitalRead(msSwitch);  // this will check the status of ms swith and if it will  open the change the leds as 
  if (a == 1)
  {
    Serial.println("CALLING OPEN TO CLOSE");
    digitalWrite(LED_A, HIGH);
    digitalWrite(LED_B, HIGH);
    digitalWrite(LED_C, HIGH);
    digitalWrite(LED_U, LOW);
    led_Astatus = 0;
    led_Bstatus = 0;
    led_Cstatus = 0;
    led_Ustatus = 2;
    count = 1;
  }

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html, processor); });

  server.on("/getledstatus", HTTP_GET, [](AsyncWebServerRequest *request) // this request will used to update response the json string from esp32 to server
            {
String json = sendtoServer();
request->send(200, "application/json", json);
json = String(); });

  // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request)  // this will be used for slider values
            {
String inputMessage1;
String inputMessage2;
// GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
if (request->hasParam(input_parameter1) && request->hasParam(input_parameter2)) {
inputMessage1 = request->getParam(input_parameter1)->value();
inputMessage2 = request->getParam(input_parameter2)->value();
// premillis=millis();
if((inputMessage1.toInt() == 5) && (inputMessage2.toInt() == 0)){
Serial.println("OFF TO ON");
sliderFlag=false;
digitalWrite(LED_U,LOW);
digitalWrite(LED_C,HIGH);
led_Ustatus=2;// 1 is for on and 0 is for off
led_Cstatus=0;
// closetoOpenFlag=false;
// opentoCloseFlag=false;

}
if((inputMessage1.toInt() == 5) && (inputMessage2.toInt() == 1)){
Serial.println("CALLING FUNCAITON");
opentoCloseFlag=false;
closetoOpenFlag=false;
sliderFlag=true;
////////////////////////////////////
premillistest=millis();
testflag=true;
    
    }




digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
}
else {
inputMessage1 = "No message sent";
inputMessage2 = "No message sent";
}
Serial.print("GPIO: ");
Serial.print(inputMessage1);
Serial.print(" - Set to: ");
Serial.println(inputMessage2);
request->send(200, "text/plain", "OK"); });

  // Start server
  server.begin();
}

void loop()
{
  deBounceFun(); // this is for ms switch debounce so we can get the status of ms switch

  if (closetoOpenFlag == true)  // if closetoopen flag is true run the following function.
  {
    mSwitchOpenToClose();  
  }

  if (opentoCloseFlag == true)  // if opentoclose flag is true run the following function. function may be inverted because on your side logic works opposite
  {
    mSwitchCloseToOpen();
  }

  if (sliderFlag == true) // this will run the slider funcation when it will change form off to on
  {
    sliderOFFtoON();
  }

  if (testflag == true) // this function will run when we change the slider from off to on and ms function working is not completed
  {
    correctoperation();
  }

  pushButtonDebounce();  // this is debounce function for push button
}
