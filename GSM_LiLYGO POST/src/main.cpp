#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "utilities.h"
#include "soc/rtc_wdt.h"
#include <ArduinoJson.h>
#define SIM800L_IP5306_VERSION_20200811


#define SerialMon Serial
#define SerialAT Serial1


////////####################################################################
#include <Wire.h>
#include <axp20x.h> 


AXP20X_Class axp;

int batPerc;
int BAT_DIS_CURR;
unsigned long prvious_powerFail=0;
float battery_voltage;
boolean powerStatus = true;
String constatus;
String lastvalue;
unsigned long previousMillis=0;

String gsm_send_serial(String command, int delay);


String deviceIMEI;
String sendtoserver;
String timeStamp;

String gsm_send_serial(String command, int delay)
{
  String buff_resp = "";
  Serial.println("Send ->: " + command);
  SerialAT.println(command);
  long wtimer = millis();
  while (wtimer + delay > millis())
  {
    if (SerialAT.available())
    {
      buff_resp = SerialAT.readString();
      Serial.println(buff_resp);
    }
  }
  Serial.println();
  return buff_resp;
}

void postdata()
{
  Serial.println("###### POSTING DATA #######");
  gsm_send_serial("AT+HTTPINIT", 2000);
  gsm_send_serial("AT+HTTPPARA=\"CID\",1", 1000);
  gsm_send_serial("AT+HTTPPARA=\"URL\",\"https://fastapi-service-5ka3ttedna-as.a.run.app/sensors/v1/sqss_insert?database_name=1anitech\"", 1000);
  gsm_send_serial("AT+HTTPPARA=\"CONTENT\",\"application/json\"", 1000);
  gsm_send_serial("AT+HTTPSSL=1", 1000);
  gsm_send_serial("AT+HTTPSSL?", 1000);
 
 // sendtoserver = "{\"deviceimei\":\"" + deviceIMEI + "\",\"temp\":\"" + (String)dht1.readTemperature() + "\",\"hum\":\"" + (String)dht1.readHumidity() + "\",\"moist\":\"" + (String)soilmoisturepercent + "\",\"relayone\":\"" + (String)relayA_status + "\",\"relaytwo\":\"" + (String)relayB_status + "\",\"csq\":\"" + SQ + "\"}"; // put your main code here, to run repeatedly:
  sendtoserver = "{\"read_date\":\"2022-12-02 12:13:14\",\"sensor_type\":\"1\",\"sensor_id\":\"1\",\"temp\":\"77\",\"humi\":\"76\",\"co2x\":\"400\",\"o2xx\":\"15.22\",\"luxx\":\"4000\"}";
  gsm_send_serial(" AT + HTTPDATA = " + String(sendtoserver.length()) + ", 100000 ", 1000);
  gsm_send_serial(sendtoserver, 1000);
  gsm_send_serial("AT+HTTPACTION=1", 1000);
  gsm_send_serial("AT+HTTPREAD", 3000);
}

void setupNTP()
{
  Serial.println("Setup NTP");
  gsm_send_serial("AT+CREG=1", 1000);
  deviceIMEI = gsm_send_serial("AT+GSN", 500);
  deviceIMEI.replace("AT+GSN", "");
  deviceIMEI.trim();
  deviceIMEI = deviceIMEI.substring(3, 20);
  deviceIMEI.trim();
  deviceIMEI = gsm_send_serial("AT+GSN", 500);
  deviceIMEI.replace("AT+GSN", "");
  deviceIMEI.trim();
  deviceIMEI = deviceIMEI.substring(0, 15);
  deviceIMEI.trim();

  gsm_send_serial("AT+SAPBR=3,1,\"Contype\",\"GPRS\"", 1000);
  gsm_send_serial("AT+SAPBR=3,1,\"APN\",\"" + (String) "apn" + "\"", 1000);
  gsm_send_serial("AT+SAPBR=1,1", 2000);
  gsm_send_serial("AT+SAPBR=2,1", 2000);
  gsm_send_serial("AT+CNTPCID=1", 1000);
  gsm_send_serial("AT+CNTP=\"europe.pool.ntp.org\",8", 3000);
  gsm_send_serial("AT+CNTP", 12000);
  gsm_send_serial("AT+CNTP", 2000);
  gsm_send_serial("AT+CCLK?", 3000);
}

void setupModem()
{

  pinMode(MODEM_PWRKEY, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  // Turn on the Modem power first
  digitalWrite(MODEM_POWER_ON, LOW);
  // Pull down PWRKEY for more than 1 second according to manual requirements
  delay(2000);
  digitalWrite(MODEM_POWER_ON, HIGH);
  digitalWrite(MODEM_PWRKEY, HIGH);
  delay(100);
  digitalWrite(MODEM_PWRKEY, LOW);
  delay(1000);
  digitalWrite(MODEM_PWRKEY, HIGH);

  // Initialize the indicator as an output
  pinMode(LED_GPIO, OUTPUT);
  digitalWrite(LED_GPIO, LED_OFF);
  delay(2000);
  // Start power management
  // Restart takes quite some time
  // To skip it, call init() instead of restart()

  // Keep reset high
  pinMode(MODEM_RST, OUTPUT);
  digitalWrite(MODEM_RST, LOW);
  delay(100);
  digitalWrite(MODEM_RST, HIGH);
  SerialMon.println("Initializing modem...");

  delay(6000);
  // Turn off network status lights to reduce current consumption
}

void setup()
{

  Serial.begin(115200);
 
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);

  setupModem();
  setupNTP();
  delay(3000);
  Wire.begin(I2C_SDA, I2C_SCL);
  int ret = axp.begin(Wire, AXP192_SLAVE_ADDRESS);
  // int ret = axp.begin(Wire);
  if (ret == AXP_FAIL)
  {
    Serial.println("AXP Power begin failed");
  //  while (1)
      ;
  }

 
}
void loop()
{

  if (millis() - previousMillis > 20000)
  {
    
    Serial.println("################################################################");

    postdata();
    previousMillis = millis();

  }

  
}