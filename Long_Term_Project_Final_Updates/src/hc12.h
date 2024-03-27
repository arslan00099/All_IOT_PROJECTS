#include <HardwareSerial.h>


HardwareSerial HC12Serial(1); // Use Serial2 for HC-12 communication

void setupHC()
{
    // Serial.begin(115200);          // Initialize serial communication for debugging
    HC12Serial.begin(9600, SERIAL_8N1, 35, 22); // Initialize HC-12 module communication (RX: GPIO35, TX: GPIO32)
}

String checkHCValue()
{
    if (HC12Serial.available()==true)
    {
        String message = HC12Serial.readStringUntil('\n'); // Read the received message
        if(message.length()>3)
       Serial.println("Received message: " + message);
        return message;
   
    }
}