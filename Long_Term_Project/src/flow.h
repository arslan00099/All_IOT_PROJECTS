
#include "Arduino.h"
#define FlowSensorPin 2
volatile long pulse;
unsigned long lastTime;
boolean flowFlag = false;
float finalLiters;
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
  if (finalLiters != preValue)
  {
    preValue = finalLiters;
    postMillis = millis();
  }

  if ((millis() - postMillis > 15000) && (finalLiters > 0))
  {
    postFlag = true;
    flowFlag = false;
    Serial.println("POSTING DATA");
  }
  if ((millis() - postMillis > 10000) && (finalLiters == 0))
  {
    resetFlag = true;
     flowFlag = false;
  }
  return finalLiters;
}
