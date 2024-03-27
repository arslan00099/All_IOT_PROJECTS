#include "fire.h"
#include <Stepper.h>
#include <Arduino.h>
#define WIFI_SSID "RIGJAZZ"
#define WIFI_PASSWORD "Kashmiri786"
#include <Esp.h>

boolean postFlag3=false;
boolean postFlag4=false;
boolean postFlag5=false;
boolean postFlag6=false;
boolean postFlag2=false;

const int steps_per_rev = 200; //Set to 200 for NIMA 17
#define IN1 5
#define IN2 4
#define IN3 14
#define IN4 12

Stepper motor(steps_per_rev, IN1, IN2, IN3, IN4);
int stepCount;
int lastsliderValue;
  int stepperSteps;

void postData(){
if (Firebase.ready())
  {
    FirebaseJson json;
      json.set("step", data[0]);
      json.set("max", data[1]);
      json.set("slider", data[2]);
      json.set("ucvalue", 0);
      json.set("dcvalue", 0);
      json.set("uvalue", 0);
      json.set("dvalue", 0);
        Firebase.RTDB.setJSONAsync(&fbdo, parentPath, &json);
   Serial.println(" UPDATED ");
    }
    else{
 Serial.println("SET AGAIN");
    }

   
}

void setup()
{Serial.begin(115200);
  motor.setSpeed(200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);


  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  setupFirebase();
 //loopF(); 
}

void loop() 
{

  if(data[3] == 1) //  UP BUTTON checking for configration
  {
  Serial.println("Rotating Clockwise...");
  motor.step(-steps_per_rev);
  stepCount--;
  data[1]--;
  Serial.println("STEP COUNT : "+(String)stepCount);
  Serial.println("data[1] : "+(String)data[1]);
  postFlag3=true;
  }
  if((data[3] == 0) && (postFlag3 == true)){
       if(data[1]<0){
      data[1]=0;
    }
    data[1]=data[1];
    data[0]=data[1];
 postData();
      postFlag3=false;
  }
  


if(data[4]== 1) // DOWN BUTTON checking for configration
{
  Serial.println("Rotating Anti-clockwise...");
  motor.step(steps_per_rev);
  data[1]++;
  stepCount++;
  Serial.println("STEP COUNT : "+(String)stepCount);
  Serial.println("DATA[1] : "+(String)data[1]);
  postFlag4=true;

  }
  if((data[4] == 0) && (postFlag4 == true)){
       data[1]=data[1];
       data[0]=data[1];
       postData();
      postFlag4=false;
  }

// WORKING BUTTONS 

  if((data[5] == 1) && (data[0] > 0)) // WORKING
  {
 // Serial.println("Rotating Clockwise...");
  motor.step(-steps_per_rev);
  data[0]--;
  Serial.println(data[0]);
  postFlag5=true;
  }
  if((data[5] == 0) && (postFlag5 == true)){
    if(data[0]<0){
      data[0]=0;
    }
 postData();
      postFlag5=false;
  }

   if((data[6] == 1)  && ((data[0] < data[1]))) // WORKING
  {
  //Serial.println("Rotating Clockwise...");
  motor.step(steps_per_rev);
  data[0]++;
  Serial.println(data[0]);
  postFlag6=true;
  }
  if((data[6] == 0) && (postFlag6 == true)){
   // data[0]=stepCount;
 postData();
      postFlag6=false;
  }


if(data[2] != lastsliderValue){
    stepperSteps = map(data[2], 0, 100, 0, data[1]); // Map the stepper position to the stepper steps range 0 to 400
Serial.println("MAP VALUE : "+(String)stepperSteps);

if(stepperSteps < data[0]){
for(int i=data[0]; i>=stepperSteps ; i--){
  Serial.println("REV Steps : "+ (String)i);
 motor.step(-steps_per_rev);
  ESP.wdtFeed(); 
}
}
if(stepperSteps > data[0]){
for(int i=data[0]; i<=stepperSteps ; i++){
  Serial.println("FOR Steps : "+ (String)i);
 motor.step(steps_per_rev);
  ESP.wdtFeed(); 
}
}

 lastsliderValue=data[2];
 postFlag2=true;
}

  if(postFlag2 == true){
    data[0]= stepperSteps;
 postData();
      postFlag2=false;
  }

   if (dataChanged)
  {
    dataChanged = false;
    stepCount=data[0];
  }
  
}

