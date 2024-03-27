
#include <WiFi.h>
#include <Arduino.h>
#include <HTTPClient.h>
String rfidSt="";

String device_id="device1";


const char* apiEndpoint = "http://192.168.18.135/tempstick/testapi.php";  // Replace with your API endpoint URL


#include <MFRC522.h>

#define SS_PIN 21  /*Slave Select Pin*/
#define RST_PIN 34 /*Reset Pin for RC522*/
#define SIZE_BUFFER 18
#define MAX_SIZE_BLOCK 16



byte nuidPICC[4] = {0, 0, 0, 0};

MFRC522::MIFARE_Key key;
// authentication return status code
MFRC522::StatusCode status;
// Defined pins to module RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); /*Create MFRC522 initialized*/

void checkFromServer() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Prepare the request payload
    String payload = "{\"deviceid\": \""+device_id+"\",\"rfid\": \""+rfidSt+"\"}";

    // Send the POST request
    http.begin(apiEndpoint);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      // Read the response
      String response = http.getString();
      Serial.println("Response:");
      Serial.println(response);
    } else {
      Serial.print("Error sending HTTP POST request. Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
    rfidSt="";
  }

 }

void printDec(byte *buffer, byte bufferSize)
{
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
    rfidSt += buffer[i];
  }
  Serial.println("\r\n########## TAG Found ######");
  Serial.println("RFID Tag Value: " + rfidSt);
  
checkFromServer();
}

void readRFID(void)
{

  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }

  if (!mfrc522.PICC_IsNewCardPresent())
    return;

  if (!mfrc522.PICC_ReadCardSerial())
    return;

  for (byte i = 0; i < 4; i++)
  {
    nuidPICC[i] = mfrc522.uid.uidByte[i];
  }
  Serial.print(F("RFID In dec: "));
  printDec(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();

  mfrc522.PICC_HaltA();

  mfrc522.PCD_StopCrypto1();
  delay(100);
}
