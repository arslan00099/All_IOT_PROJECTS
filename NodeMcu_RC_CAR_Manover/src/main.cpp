#include <Arduino.h>
#include "firebas.h"
#define PWMA D1
#define PWMB D2
#define DRA D3
#define DRB D4
#define DRCF D5
#define DRCB D6
#define DRDF D7
#define DRDB D8

void stop()
{
  analogWrite(PWMA, LOW);
  digitalWrite(DRA, LOW);
  analogWrite(PWMB, LOW);
  digitalWrite(DRB, LOW);
  analogWrite(DRCF, 0);
  digitalWrite(DRCB, LOW);
  analogWrite(DRDF, 0);
  digitalWrite(DRDB, LOW);
}
void moveForward()
{
  analogWrite(PWMA, pulseVal - 15);
  analogWrite(PWMB, pulseVal - 15);
  digitalWrite(DRA, HIGH);
  digitalWrite(DRB, HIGH);

  analogWrite(DRCF, pulseVal);
  digitalWrite(DRCB, LOW);
  analogWrite(DRDB, pulseVal);
  digitalWrite(DRDF, LOW);
}

void moveBack()
{
  analogWrite(PWMA, pulseVal - 15);
  analogWrite(PWMB, pulseVal - 15);
  digitalWrite(DRA, LOW);
  digitalWrite(DRB, LOW);
  analogWrite(DRCB, pulseVal);
  digitalWrite(DRCF, LOW);
  analogWrite(DRDF, pulseVal);
  digitalWrite(DRDB, LOW);
}
void moveLeft()
{
  analogWrite(PWMA, pulseVal - 15);
  analogWrite(PWMB, pulseVal - 15);
  digitalWrite(DRA, LOW);
  digitalWrite(DRB, HIGH);
  analogWrite(DRCF, pulseVal);
  digitalWrite(DRCB, LOW);
  analogWrite(DRDF, pulseVal);
  digitalWrite(DRDB, LOW);
}
void moveRight()
{
  analogWrite(PWMA, pulseVal - 15);
  analogWrite(PWMB, pulseVal - 15);
  digitalWrite(DRA, HIGH);
  digitalWrite(DRB, LOW);

  analogWrite(DRCB, pulseVal);
  digitalWrite(DRCF, LOW);
  analogWrite(DRDB, pulseVal);
  digitalWrite(DRDF, LOW);
}

void moveLeftRound()
{
  analogWrite(PWMA, pulseVal - 15);
  analogWrite(PWMB, 0);
  digitalWrite(DRA, HIGH);
  analogWrite(DRCF, pulseVal);
  digitalWrite(DRCB, LOW);
}
void moveRightRound()
{
  analogWrite(PWMA, 0);
  analogWrite(PWMB, pulseVal - 15);
  digitalWrite(DRB, LOW);
  analogWrite(DRDF, pulseVal);
  digitalWrite(DRDB, LOW);
}
void setup()
{
  // put your setup code here, to run once:
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  setupFB();
}

void loop()
{
  loopFB();
}