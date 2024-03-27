#include <Arduino.h>
#include <WiFi.h>
#include "utilities.h"
#include "soc/rtc_wdt.h"
#define SIM800L_IP5306_VERSION_20200811

#define SLED_PIN 19
#define BUZZ_PIN 18
boolean relayA_status = false;
boolean relayB_status = false;
#define SerialMon Serial
#define SerialAT Serial1

String gsm_send_serial(String command, int delay);

unsigned previousMillisRelay = 0;
unsigned previousMillisdht = 0;
String deviceIMEI;

    String
    gsm_send_serial(String command, int delay)
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

//##################################  GET REQUEST FUNCTION ########################

void getRequest()
{

  gsm_send_serial("AT+HTTPINIT", 2000);
  gsm_send_serial("AT+HTTPPARA=\"CID\",1", 1000);
  gsm_send_serial("AT+HTTPPARA=\"URL\",\"https://fastapi-service-5ka3ttedna-as.a.run.app/sensors/v1/sqss_insert?database_name=1anitech\"", 1000);  //LINK
  gsm_send_serial("AT+HTTPSSL=1", 1000);    // Commands to activate SSL
  gsm_send_serial("AT+HTTPSSL?", 1000);
  gsm_send_serial("AT+HTTPACTION=1", 8000);
  String payload = gsm_send_serial("AT+HTTPREAD", 3000);
  Serial.println("->" + payload);  //Print payload
}

void setup()
{

  Serial.begin(115200);

  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
 
  setupModem();
  setupNTP();
  delay(3000);
  Wire.begin(I2C_SDA, I2C_SCL);
}
void loop()
{

  if (millis() - previousMillisRelay > 5000)
  {
    //
    Serial.println("#######   GET REQUEST   ####");
    getRequest();

    previousMillisRelay = millis();
  }
}