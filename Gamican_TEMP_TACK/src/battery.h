#include <Arduino.h>

const int batteryPin = 35;  // GPIO pin connected to the battery voltage divider
const int numReadings = 10; // Number of readings for the moving average filter
float filteredVoltage = 0;
float batteryPercentage = 0;
float readings[numReadings]; // Array to store raw readings for filtering
int readIndex = 0;           // Current index in the readings array
float total = 0;             // Running total of readings for the moving average

void setupBattery()
{

    // Initialize the readings array with zeros
    pinMode(batteryPin,INPUT);
    for (int i = 0; i < numReadings; i++)
    {
        readings[i] = 0.0;
    }
}

void getPercentBatt()
{
    // Read the analog voltage from the battery pin
    for (int i = 0; i < 10; i++)
    {
        int rawValue = analogRead(batteryPin);
Serial.println(rawValue);
        // Convert raw value to voltage (assuming 12-bit ADC and 3V reference)
        float voltage = (float)rawValue * (3.0 / 4095.0);

        // Add the new reading to the total and remove the oldest reading
        total = total - readings[readIndex];
        readings[readIndex] = voltage;
        total = total + readings[readIndex];

        // Move to the next reading index
        readIndex++;
        if (readIndex >= numReadings)
        {
            readIndex = 0;
        }

        // Calculate the filtered voltage
        filteredVoltage = total / numReadings;

        // Calculate battery percentage with hysteresis
        batteryPercentage = (filteredVoltage / 3.0) * 100.0;
  
    }
    Serial.print("Battery Voltage: ");
    Serial.print(filteredVoltage, 2);
    Serial.print("V    Battery Percentage: ");
    Serial.print(batteryPercentage, 2);
    Serial.println("%");
}