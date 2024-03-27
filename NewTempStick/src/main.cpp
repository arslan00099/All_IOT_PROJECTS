#include <Arduino.h>
#include "esp32SmartConf.h"
#include <WiFi.h>
#define BAT_PIN 26
unsigned long adcValue;

RTC_DATA_ATTR int bootCount = 0;


void smartConfig()
{
  WiFi.beginSmartConfig();
  while (!WiFi.smartConfigDone())
  {
    // Serial.print("sc|");
    delay(100);
  }
  Serial.println("   Config Received  ");
  delay(3000);

  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    // Serial.print(".");
  }
  String ssid = WiFi.SSID();
  String password = WiFi.psk();
  credantials.begin("credential", false);
  credantials.getString("ssid", ssid);
  credantials.getString("password", password);
  credantials.end();
}

void measureVoltage() {
  // Read the voltage on pin 26
  adcValue = analogRead(BAT_PIN);
  
  // Convert ADC value to voltage (assuming 3.3V reference voltage)
  float voltage = (adcValue / 4095.0) * 3.3;
  
 
  Serial.print(voltage, 2); // Display voltage with 2 decimal places
  Serial.println(" V");
  
  delay(1000); // Delay for readability
}


void setup()
{
  pinMode(BAT_PIN,INPUT);
  analogReadResolution(12);
  Serial.begin(115200);
  while (1)
  {
      measureVoltage();
  }
  
  // pinMode(STATUS_RED, OUTPUT);
  // pinMode(STATUS_GREEN, OUTPUT);
  // pinMode(STATUS_BLUE, OUTPUT);
  // Serial.println("GPIO 0: " + (String)digitalRead(0));
  // turnOnCircuit();
  // setupBME();
  // setupBattery();
  // getPercentBatt();
  // print_wakeup_reason();

  delay(1000);

  // Reading WiFi Credential from NVS
  credantials.begin("credential", false);
 // credantials.clear();
  String WiFi_SSID = credantials.getString("ssid");
  String WiFi_PASS = credantials.getString("password");
  String Email=credantials.getString("email");
  credantials.end();
  Serial.println("WiFi_SSID: " + (String)WiFi_SSID);
  Serial.println("WiFi_PASS: " + (String)WiFi_PASS);
  Serial.println("Email: " + (String)Email);
  if (WiFi_SSID.length() > 2)
  {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    Serial.println("Connecting to WiFi...");
    WiFi.begin(WiFi_SSID.c_str(), WiFi_PASS.c_str());
    unsigned long pre_time = 0;
    pre_time = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - pre_time <= 10000))
    {
      Serial.print("..");

    //  WIFI_CONNECT();
      delay(200);
     // ALL_OFF();
      delay(1000);
    }

    if (bootCount == 0 || bootCount % 50 == 0)
    {

      if (WiFi.status() == WL_CONNECTED)
      {

     //   setupOTA();
        if (bootCount % 50 == 0)
        {
          bootCount = 1;
        }
      }
    }
  }
  else
  {
    // smartConfig(); // While not get SSID PSWD
   //WIFI_MISSING();
 //  RESET_CREDENTIALS();
    setupSmartConfig();
    Serial.println("Starting smartConfig");
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("  Failed to Connect! ");
    delay(3000);
    smartConfig();
   // setupSmartConfig();
  }
  else
  {
//ALL_OFF();
//WIFI_POST();
    Serial.println("    Connected :)     ");
//ALL_OFF();
    // setting up NTP and MQTT servers
  //  postData();
  }
  // postData();

  //setupDeepSleep();
}

void loop() {

}

