#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <MFRC522.h> //library responsible for communicating with the module RFID-RC522
#include <SPI.h>     //library responsible for communicating of SPI bus
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

#define cell1 2
#define cell2 26
#define SS_PIN 21
#define RST_PIN 22
#define SIZE_BUFFER 18
#define MAX_SIZE_BLOCK 16
#define greenPin 12
#define redPin 2
#define SDA_2 33
#define SCL_2 32
#define btn1_PIN  13
#define btn2_PIN  12
// #define btn3_PIN  14
// #define btn4_PIN  27
byte nuidPICC[4] = {0, 0, 0, 0};
String rfidSt = "";
int item=0;
// used in authentication
MFRC522::MIFARE_Key key;
// authentication return status code
MFRC522::StatusCode status;
// Defined pins to module RC522
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;
int pos = 0; // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
int servo1Pin = 4;

const char *ssid = "RIGJAZZ";
const char *password = "Kashmiri786";

const char *serverName = "http://espstack.com/vendingMachine/getapi.php";

void getData(int item){
  WiFiClient client;
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);

 
  // If you need an HTTP request with a content type: application/json, use the following:
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST("{\"rfid\":\""+rfidSt+"\",\"cell\":\""+(String)item+"\"}");

  // If you need an HTTP request with a content type: text/plain
  // http.addHeader("Content-Type", "text/plain");
  // int httpResponseCode = http.POST("Hello, World!");

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  // httpResponseCode = http.GET();

  if (httpResponseCode > 0)
  {
      String payload = http.getString();
    Serial.println(payload);
    if (payload.indexOf("Sucess!") > -1)
    {
      Serial.println("SUCESS");
      if (item == 1)
      {
        digitalWrite(cell1, LOW);
        Serial.println("ITEM 1");
        delay(500);
        digitalWrite(cell1, HIGH);
      }
      if (item == 2)
      {
        digitalWrite(cell2, LOW);
        Serial.println("ITEM 2");
        delay(500);
        digitalWrite(cell2, HIGH);
      }
    }

    lcd.clear();
    lcd.print((String)payload);
    if(payload.length()>16){
      lcd.setCursor(0,1);
      payload.remove(0,16);
      lcd.print((String)payload);
    }
    delay(5000);
    lcd.clear();
    lcd.print("  THANK YOU!");
    delay(5000);
    lcd.clear();
    lcd.print("SWIPE YOUR CARD");
    rfidSt = "";
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}

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
  lcd.clear();
  lcd.print(rfidSt);
 // getData();
  // //////printOLED(0, 0, "RFID:\r\n" + (rfidSt));
}

void readRFID(void)
{ /* function readRFID */
  ////Read RFID card
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
  // Look for new 1 cards
  if (!mfrc522.PICC_IsNewCardPresent())
    return;
  // Verify if the NUID has been readed
  if (!mfrc522.PICC_ReadCardSerial())
    return;
  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++)
  {
    nuidPICC[i] = mfrc522.uid.uidByte[i];
  }
  Serial.print(F("RFID In dec: "));
  printDec(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
  // Halt PICC
  mfrc522.PICC_HaltA();
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
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


void scanner(){
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
      {
        Serial.print("0");
      }
      Serial.println(address, HEX);
      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknow error at address 0x");
      if (address < 16)
      {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
  {
    Serial.println("No I2C devices found\n");
  }
  else
  {
    Serial.println("done\n");
  }
  delay(5000);
}

void setupWifi(){
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void setupServo(){
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50); // standard 50 hz servo
  myservo.attach(servo1Pin, 500, 2400);
}
void setup()
{
  pinMode(cell1, OUTPUT);
  pinMode(cell2, OUTPUT);
  digitalWrite(cell1, HIGH);
  digitalWrite(cell2, HIGH);
  lcd.init(); // initialize the lcd
  lcd.backlight();
  Wire.begin(SDA_2,SCL_2);
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  setupWifi();
  setupServo();
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);

  // Init MFRC522
  mfrc522.PCD_Init();
  Serial.println("Approach your reader card...");
  Serial.println();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("     WELCOME");
  delay(5000);
  lcd.clear();
  lcd.print("SWIPE YOUR CARD");
  pinMode(btn1_PIN,INPUT_PULLUP);
  pinMode(btn2_PIN, INPUT_PULLUP);
  // pinMode(btn3_PIN, INPUT_PULLUP);
  // pinMode(btn4_PIN, INPUT_PULLUP);
  
  // while (1)
  // {
  //   digitalWrite(cell1,LOW);
  //   Serial.println("LOW");
  //   delay(100);
  //   digitalWrite(cell1,HIGH);
  //   Serial.println("HIGH");
  //   delay(8000);

  //   digitalWrite(cell2, LOW);
  //   Serial.println("LOW");
  //   delay(100);
  //   digitalWrite(cell2, HIGH);
  //   Serial.println("HIGH");
  //   delay(8000);
  // }
  
}

void loop()
{
  readRFID();
 // scanner();
//Serial.println(digitalRead(btn1_PIN));
  if ((digitalRead(btn1_PIN) == 0) && (rfidSt.length()>0))
  {
    Serial.println("BTN1");
   
    getData(1);
  }
  if ((digitalRead(btn2_PIN) == 0) && (rfidSt.length() > 0))
  {
    Serial.println("BTN2");
    getData(2);
 }
//  if ((digitalRead(btn3_PIN) == 0) && (rfidSt.length() > 0))
//  {
//   Serial.println("BTN3");
//   getData(3);
//  }
//  if ((digitalRead(btn4_PIN) == 0) && (rfidSt.length() > 0))
//  {
//   Serial.println("BTN4");
//   getData(4);
//  }


}