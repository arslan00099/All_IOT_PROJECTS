#include <Arduino.h>

#include <Wire.h>
boolean Flag=LOW;
boolean to_send=LOW;
int button=3;
int out=13;
int counter=0; //pulse counter
unsigned int long startt,stopp=0,startt_2,stopp_2;
String x="";
int board_number=1;
String p="";
char dummy[20];
int pulses=0;

void Pulse()
{ 
  startt = millis();
  if (startt-stopp>=300)
  {
    if ((Flag==LOW) && (to_send==LOW))
    {
      counter += 100;
      stopp=startt;
    }
  } 
}

void butn()
{
  startt_2 = millis();
  if (to_send==LOW)
  {
  if (startt_2-stopp_2>=300)
  {
    Serial.println("Button detected");
    Flag=HIGH;
    stopp_2=startt_2;
  }
  } 

}

void handler()
{
  Wire.write(x.c_str());
  Flag=LOW;
  counter=0;
}

void receive(int howmany)
{
  p="";
  while (Wire.available())
    {
      p+= (char)Wire.read();  
    }
  p.toCharArray(dummy, 20);
  pulses = atoi(dummy);
  to_send=HIGH;
}




void setup() 
{
     Serial.begin(115200);
   pinMode(21,OUTPUT);
   pinMode(36,INPUT_PULLUP);
   pinMode(16,INPUT_PULLUP);
   attachInterrupt(digitalPinToInterrupt(36),butn,RISING);
   attachInterrupt(digitalPinToInterrupt(16),Pulse,FALLING);
   Wire.begin(board_number);
   Wire.onRequest(handler);
   Wire.onReceive(receive);

}

void loop() 
{
  if ((Flag==HIGH) && (to_send==LOW))// if button is pressed
  {
    x="B:"+String(board_number);

  }
  if ((Flag==LOW) && (to_send==LOW))
  {
    x="C:"+String(counter);
    //Serial.println(x);
  }
  if (to_send==HIGH)
  {
    for (int i=pulses;i>=1;i--)
    {
      digitalWrite(2,HIGH);
      delay(3);// change duarion or time of pulse
      digitalWrite(2,LOW);
      delay(3);
    }
    pulses=0;
    to_send=LOW;
  }
}


