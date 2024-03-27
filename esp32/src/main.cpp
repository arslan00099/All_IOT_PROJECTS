#include <WiFi.h>
#include "index.html"

// Replace with your network credentials
const char* ssid = "Neo";
const char* password = "12345678";

const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";
int timeForPhoto=0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);



String outputState(int output){
  if(digitalRead(output)){
    return "checked";
  }
  else {
    return "";
  }
}



// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons = "";
    buttons +="<div style=\"display: flex;\">";
    buttons += "<h4>Output - GPIO 2  :&nbsp;&nbsp;&nbsp; </h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" " + outputState(2) + "><span class=\"slider\"></span></label>";
    buttons += "</div>";
    buttons +="<div style=\"display: flex;\">";
    buttons += "<h4>Output - GPIO 4  :&nbsp;&nbsp;&nbsp; </h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\" " + outputState(4) + "><span class=\"slider\"></span></label>";
    buttons += "</div>";
    buttons +="<div style=\"display: flex;\">";
    buttons += "<h4>Output - GPIO 33 :&nbsp;&nbsp; </h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"33\" " + outputState(33) + "><span class=\"slider\"></span></label>";
    buttons += "</div>";
    buttons +="<div style=\"display: flex;\">";
    buttons += "<h4>Output - GPIO 12  :&nbsp;&nbsp; </h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"12\" " + outputState(12) + "><span class=\"slider\"></span></label>";
    buttons += "</div>";
    return buttons;
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  pinMode(33, OUTPUT);
  digitalWrite(33, LOW);
    pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      if(inputMessage1.indexOf('time')>-1){
Serial.println("TIME IS : "+(String)inputMessage2+"mints");
timeForPhoto=inputMessage2.toInt();
      }else{
      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
      }
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();
}

void loop() {

}