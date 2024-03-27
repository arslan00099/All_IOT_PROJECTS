#include <Arduino.h>
const int trigPin = 26;
const int echoPin = 27;
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

void setupSonar()
{     
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT);  // Sets the echoPin as an Input
}

float getSonarReadings()
{
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // measure duration of pulse from ECHO pin
    duration= pulseIn(echoPin, HIGH);

    // calculate the distance
    distanceCm = 0.017 * duration;

    // print the value to Serial Monitor
    // Serial.print("distance: ");
    // Serial.print(distanceCm);
    // Serial.println();

    return (float)distanceCm;
}