#include <NTPClient.h>
// change next line to use with another board/shield
#include <WiFi.h>
//#include <WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>
#include <BluetoothSerial.h>
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

//const char *ssid = "RIGJAZZ";
//const char *password = "Kashmiri786";
        const char  *ssid = "WPS_Phone2";  // "RIGJAZZ"                     // "WPS_Phone2"
const char  *password = "ingo9100";  // "Kashmiri786"                 // "ingo9100"while (WiFi.status() != WL_CONNECTED)
       
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 2        /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0;

/*
Method to print the reason by which ESP32
has been awaken from sleep
*/
void setupNTP();
void loopNTP();
void print_wakeup_reason()
{
    esp_sleep_wakeup_cause_t wakeup_reason;

    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch (wakeup_reason)
    {
    case ESP_SLEEP_WAKEUP_EXT0:
        Serial.println("Wakeup caused by external signal using RTC_IO");
        break;
    case ESP_SLEEP_WAKEUP_EXT1:
        Serial.println("Wakeup caused by external signal using RTC_CNTL");
        break;
    case ESP_SLEEP_WAKEUP_TIMER:
        Serial.println("Wakeup caused by timer");
        delay(1000);
        Serial.println("setup BLuetooth");
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");


        break;

    case ESP_SLEEP_WAKEUP_TOUCHPAD:
        Serial.println("Wakeup caused by touchpad");
        break;
    case ESP_SLEEP_WAKEUP_ULP:
        Serial.println("Wakeup caused by ULP program");
        break;
    default:
        Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
        clearMatrix();
        displayMatrix(0, 0, 1, matrix.Color333(7, 4, 0), "Connecting WiFi");
        //WiFi.begin("RIGJAZZ","Kashmiri786");
        

       WiFi.begin(ssid,password);
        {
            Serial.print(".");
            displayMatrix(0, 0, 1, matrix.Color333(7, 4, 0), ".");
            delay(1250);
        }

        // Oled Screen Startup 0.96-inch 128*64 dot matrix OLED
        matrix.fillScreen(matrix.Color333(0, 0, 0));
        clearMatrix();
        displayMatrix(0, 0, 1, matrix.Color333(0, 7, 0), "Connected");
        Serial.println("Connected");
        Serial.println(WiFi.localIP());
        Serial.println("DevID: " + (WiFi.macAddress()));
        timeClient.begin();
        delay(2000);
        loopNTP();
        setUnixtime(timeClient.getEpochTime());
          ++bootCount;
        Serial.println("Boot number: " + String(bootCount));
   esp_deep_sleep_start();
  Serial.println("This will never be printed");
        break;
    }
}

void setupNTP()
{
    Serial.begin(115200);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    timeClient.begin();
}

void loopNTP()
{
    while (timeClient.forceUpdate() == false)
    {
        delay(1000);
    }

    Serial.println(timeClient.getFormattedTime());
    Serial.println(timeClient.getEpochTime());
    delay(1000);
}
