#include <Arduino.h>
#define BLYNK_TEMPLATE_ID "TMPL6b8sjWB_z"
#define BLYNK_TEMPLATE_NAME "DriverSleep"
#define BLYNK_AUTH_TOKEN "Libj-ySoiCoOmLD3XcJVXuuCEwabirdG"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#define Buzzer_PIN 14
#define irSensor_PIN 13
#define relay_PIN 27
#define pushButton_PIN 25
#define led_PIN 2
unsigned long premillisBlink=0;
int previousState = LOW;
bool objectDetected = false;
int i=0;


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Islo AI";
char pass[] = "gamican3506";

BlynkTimer timer;

BLYNK_WRITE(V0)
{
  int pinValue=param.asInt();
if(pinValue == 0){
digitalWrite(Buzzer_PIN,LOW);
digitalWrite(led_PIN,LOW);
digitalWrite(relay_PIN,LOW);
premillisBlink=millis();
i=0;
}
  }

void setup()
{
  Serial.begin(115200);
  pinMode(irSensor_PIN,INPUT);
  pinMode(relay_PIN,OUTPUT);
  pinMode(Buzzer_PIN,OUTPUT);
  pinMode(led_PIN,OUTPUT);
  pinMode (pushButton_PIN,INPUT_PULLUP);


Blynk.begin(auth, ssid, pass);
Blynk.virtualWrite(V0,0);
}

void loop()
{
  Blynk.run();
   int irState = digitalRead(irSensor_PIN);
  
  // Check if the state has changed
  if (irState != previousState) {
    previousState = irState;
    
    // Print the state
    if (irState == HIGH) {
       Serial.println("No object detected.");
    i=0;
         premillisBlink=millis();
         int post=millis()-premillisBlink;
 Blynk.virtualWrite(V1,post);
      objectDetected = true;
    } else {
       Serial.println("Object detected!");
        premillisBlink=millis();
        i=0;
          int post=millis()-premillisBlink;
 Blynk.virtualWrite(V1,post);
      objectDetected = false;
    }
  }


if(millis()-premillisBlink > 10000){
Serial.println("GREATER THEN 10 SEC...");
 Blynk.virtualWrite(V1,millis());

digitalWrite(Buzzer_PIN,HIGH);
digitalWrite(led_PIN,HIGH);
digitalWrite(relay_PIN,HIGH);
if(i==0){
  Blynk.virtualWrite(V0,1);
  i++;
}
}


 // Serial.println(digitalRead(pushButton_PIN));
  if(digitalRead(pushButton_PIN) == 0){
   Blynk.virtualWrite(V0,0);
digitalWrite(Buzzer_PIN,LOW);
digitalWrite(led_PIN,LOW);
digitalWrite(relay_PIN,LOW);
premillisBlink=millis(); 
 int post=millis()-premillisBlink;
 Blynk.virtualWrite(V1,post);
  }
 
 // delay(500);

}