
#include "Arduino.h"
#define FlowSensorPin 4
volatile long pulse;
unsigned long lastTime;
boolean flowFlag = false;

unsigned long postMillis = 0;
boolean postFlag = false;
float preValue = 0;
boolean resetFlag = false;

float volume;

ICACHE_RAM_ATTR void increase()
{
  pulse++;
  
}

void setupFlowSensor()
{
  pinMode(FlowSensorPin, INPUT);

  attachInterrupt(digitalPinToInterrupt(FlowSensorPin), increase, RISING);
}

float CheckFlow(float calibF)
{
  calibF=1;
  volume = calibF * pulse / 1000 * 30;
  if (millis() - lastTime > 2000)
  {
    pulse = 0;
  }

  finalLiters += volume;
  Serial.print(finalLiters);
  Serial.println(" L/m");
  prevMillisRef=millis();
  if (finalLiters != preValue)
  {
    preValue = finalLiters;
    postMillis = millis();
     Serial.println(postMillis);
  }
  else
  {
    Serial.println(postMillis);
  }

  if ((millis() - postMillis > relayTimeOut*1000) && (finalLiters > 0))
  {
    postFlag = true;
    resetFlag = true;
     flowFlag = false;

  }
  if ((millis() - postMillis > relayTimeOut*1000) && (finalLiters == 0))
  {
    resetFlag = true;
     flowFlag = false;
  }
  
  return finalLiters;
}
