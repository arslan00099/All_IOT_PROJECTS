// Libraries
#include <Arduino.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <SPI.h>         //https://www.arduino.cc/en/reference/SPI
#include <MFRC522.h>     //https://github.com/miguelbalboa/rfid

#include "httpPost.h"
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
String rfidSt = "";
TaskHandle_t Task1;
TaskHandle_t Task2;
HTTPClient http;
// Constants
#define SS_PIN 18
#define RST_PIN 22
// Parameters
const int ipaddress[4] = {103, 97, 67, 25};
// Variables
byte nuidPICC[4] = {0, 0, 0, 0};
MFRC522::MIFARE_Key key;
MFRC522 rfid = MFRC522(SS_PIN, RST_PIN);
void printDec(byte *buffer, byte bufferSize);
int latBuff=0;
int lon=0;
void readRFID(void)
{ /* function readRFID */
  ////Read RFID card
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
  // Look for new 1 cards
  if (!rfid.PICC_IsNewCardPresent())
    return;
  // Verify if the NUID has been readed
  if (!rfid.PICC_ReadCardSerial())
    return;
  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++)
  {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }
  Serial.print(F("RFID In dec: "));
  printDec(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();
  // Halt PICC
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
  delay(500);
}
/**
    Helper routine to dump a byte array as hex values to Serial.
*/
void printHex(byte *buffer, byte bufferSize)
{
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
/**
    Helper routine to dump a byte array as dec values to Serial.
*/
void printDec(byte *buffer, byte bufferSize)
{
  rfidSt = "";
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
    rfidSt += buffer[i];
  }
  Serial.println("\r\n########## TAG Found ######");
  Serial.println("RFID Tag Value: " + rfidSt);
 // //////printOLED(0, 0, "RFID:\r\n" + (rfidSt));
  logCount++;
}
void postData()
{
  String httpRequestData = "API_KEY=379d9260-a965-4c05-8a33-240c1143e2e0&rfid=" + rfidSt + "&lat=" + (String)latBuff + "&lng=" + (String)lon + "&deviceId=" + (String)WiFi.macAddress();

  if ((WiFi.status() == WL_CONNECTED))
  { // Check the current connection status

    Serial.println("POSTING DATA");
    http.begin("https://us-central1-school-management-863cd.cloudfunctions.net/studentAttendance", ca_cert); // Specify the URL and certificate
    // http.begin("http://us-central1-school-management-863cd.cloudfunctions.net/studentAttendance"); //Specify the URL and certificate

    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    // Data to send with HTTP POST
    // Send HTTP POST request
    int httpCode = http.POST(httpRequestData);
    // Make the request

    if (httpCode > 0)
    { // Check for the returning code

      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
      if (httpCode == 200)
      {
        Serial.println("Successfully POSTED");
        //////printOLED(0, 0, "AttendanceMarked");
        logCount--;
        if (logCount < -1)
          logCount = 0;
      }
    }

    else
    {
      Serial.println("Error on HTTP request");
      //////printOLED(0, 0, "Post\r\nError");
    }
    //////printOLED(0, 32, "lat:" + (String)lat + "\r\n" + "lat:" + (String)lon);
    http.end(); // Free the resources
  }
}
void Task1code(void *pvParameters)
{
  Serial.print(" Stepper Task running on core");
  Serial.println(xPortGetCoreID());

  for (;;)
  {

    readRFID();
    TIMERG1.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
    TIMERG1.wdt_feed = 1;
    TIMERG1.wdt_wprotect = 0;
    vTaskDelay(10);
  }
}

// Task2code: blinks an LED every 700 ms
void Task2code(void *pvParameters)
{
  Serial.print(" MQTT Task running on core");
  Serial.println(xPortGetCoreID());

  for (;;)
  {
    //getLocation();

    // //////printOLED(0,0,gps_raw);
    delay(2000);
    if (logCount > 0)
    {
   //   postData();
    }
    vTaskDelay(10);
    TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
    TIMERG0.wdt_feed = 1;
    TIMERG0.wdt_wprotect = 0;
  }
}

void setup()
{
  // Init Serial USB
  Serial.begin(115200);
  // latBuff[0]=lat;
  // lonBuff[0]=lon;
  WiFiManager wm;

  // wm.resetSettings();

  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  delay(2000);
  //////printOLED(0, 0, "SoftAP\r\n192.168.4.1");
  res = wm.autoConnect("AutoConnectAP", "password"); // password protected ap

  if (!res)
  {
    Serial.println("Failed to connect");
    // ESP.restart();
    //////printOLED(0, 0, "Soft AP");
  }
  else
  {
    // if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
    Serial.println("");
    Serial.print("WiFi connected to: ");
    Serial.println(wm.getWiFiSSID());
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    //////printOLED(0, 0, "WiFi\r\nconnected");
    postData();
  }

  Serial.println(F("Initialize System"));

  // init rfid D8,D5,D6,D7
  SPI.begin();
  rfid.PCD_Init();
  Serial.print(F("Reader :"));
  rfid.PCD_DumpVersionToSerial();

 
  xTaskCreatePinnedToCore(
      Task1code, /* Task function. */
      "Task1",   /* name of task. */
      10000,     /* Stack size of task */
      NULL,      /* parameter of the task */
      1,         /* priority of the task */
      &Task1,    /* Task handle to keep track of created task */
      0);        /* pin task to core 0 */
  delay(100);

  // create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
      Task2code, /* Task function. */
      "Task2",   /* name of task. */
      10000,     /* Stack size of task */
      NULL,      /* parameter of the task */
      1,         /* priority of the task */
      &Task2,    /* Task handle to keep track of created task */
      1);        /* pin task to core 1 */
  delay(100);
}
void loop()
{
}