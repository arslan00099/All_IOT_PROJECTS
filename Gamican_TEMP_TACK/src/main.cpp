
#include <Arduino.h>
#include <Preferences.h>
float tempF = 0.0;
unsigned int hum = 0;
unsigned int press = 0;
Preferences credantials;
#define SLEEP_PIN GPIO_NUM_4
#define BATT_PIN 35
//------ Libraries --------//
#include "Device_status.h"
#include "esp32SmartConf.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "OTA.h"
#include "esp32_deepSleep.h"
#include <esp_smartconfig.h>
#include "BME280_sens.h"
#include "battery.h"
#define LED_POST 5
#define RST_CONFIG 0

unsigned long prevMillisRST = 0;
const char *serverUrl = "http://espstack.com/tempstick/api/post.php";
// int offset = 3600 * 5;

//-------------------------//
void callback(char *topic, byte *payload, unsigned int length);
void publishMessage(const char *topic, String payload, boolean retained);

//------ Objects -----------//
WiFiClientSecure espClient;

DynamicJsonDocument doc(1024); // 1024, JSON document size in Bytes
//-------------------------//

//---- Global Var ---------//
int8_t minTempAlert = -40;
int8_t maxTempAlert = 80;
String time_zone = "";
// char temp[10] = "";

//-------------------------//
unsigned long getTime()
{
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    // Serial.println("Failed to obtain time");
    return (0);
  }
  time(&now);
  return now;
}

void obtainTime(int offset)
{
  configTime(offset, 0, "pool.ntp.org"); // Configure NTP time synchronization

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour, 3, "%H", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay, 10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println();
}
void postData()
{
  // Create JSON object
  DynamicJsonDocument jsonDoc(500);
  //{"deviceid":"ttt","tempin":"60","humi":"10","pressure":"10","epouch":"1234567","gmtoffset":"200","ssid":"RIGJAZZ","rssi":"10","battery":"90","firmware":"1.1"}
  obtainTime(0);

  jsonDoc["device_id"] = "TTDV1";
  jsonDoc["temp"] = tempF;
  jsonDoc["humi"] = hum;
  jsonDoc["pressure"] = 25.5;
  jsonDoc["email"] = "admin@admin.com";
  jsonDoc["ssid"] = WiFi.SSID();
  jsonDoc["rssi"] = WiFi.RSSI();
  jsonDoc["epouch"] = getTime();
  jsonDoc["battery"] = batteryPercentage;
  jsonDoc["firmware"] = FirmwareVer;
  // Serialize JSON to a string
  String jsonString;
  serializeJson(jsonDoc, jsonString);
  Serial.println("Posting Data: " + (jsonString));
  // Send HTTP POST request
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(jsonString);

  // Check for successful request
  if (httpResponseCode == HTTP_CODE_OK)
  {
    String response = http.getString();
    Serial.println("HTTP POST request successful");
    Serial.println("Server response: " + response);
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

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
void turnOnCircuit()
{
  // delay(1000);
  // gpio_pad_select_gpio(SLEEP_PIN);
  // gpio_set_direction(SLEEP_PIN, GPIO_MODE_OUTPUT);
  // delay(1000);
  printf("Turning on Circuit \n");
  // gpio_set_level(SLEEP_PIN, 1);
  pinMode(SLEEP_PIN, OUTPUT);
  digitalWrite(SLEEP_PIN, HIGH);
}
void deepSleep()
{

  printf("Restarting now.\n");
  fflush(stdout);
  // gpio_deep_sleep_hold_en();
  // gpio_hold_en(SLEEP_PIN);
  esp_sleep_enable_timer_wakeup(5000000);
  esp_deep_sleep_start();
  // Delay to ensure the pin is set before entering sleep mode
}

void setup()
{
  Serial.begin(115200);
  pinMode(STATUS_RED, OUTPUT);
  pinMode(STATUS_GREEN, OUTPUT);
  pinMode(STATUS_BLUE, OUTPUT);
  Serial.println("GPIO 0: " + (String)digitalRead(0));
  turnOnCircuit();
  setupBME();
  setupBattery();
  getPercentBatt();
  print_wakeup_reason();

  delay(1000);

  // Reading WiFi Credential from NVS
  credantials.begin("credential", false);
  String WiFi_SSID = credantials.getString("ssid");
  String WiFi_PASS = credantials.getString("password");
  credantials.end();
  Serial.println("WiFi_SSID: " + (String)WiFi_SSID);
  Serial.println("WiFi_PASS: " + (String)WiFi_PASS);
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

      WIFI_CONNECT();
      delay(200);
      ALL_OFF();
      delay(1000);
    }

    if (bootCount == 0 || bootCount % 50 == 0)
    {

      if (WiFi.status() == WL_CONNECTED)
      {

        setupOTA();
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
   RESET_CREDENTIALS();
    setupSmartConfig();
    Serial.println("Starting smartConfig");
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("  Failed to Connect! ");
    delay(3000);
  }
  else
  {
//ALL_OFF();
WIFI_POST();
    Serial.println("    Connected :)     ");
ALL_OFF();
    // setting up NTP and MQTT servers
    postData();
  }
  // postData();

  setupDeepSleep();
}
void loop()
{
}
