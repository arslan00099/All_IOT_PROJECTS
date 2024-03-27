#include <Arduino.h>
#include <WiFi.h>
#include <ESPNtpClient.h>

// Your WiFi credentials
#define YOUR_WIFI_SSID "Islo AI"
#define YOUR_WIFI_PASSWD "gamican3506"

// Onboard LED pin
#ifdef ESP32
#define ONBOARDLED 5 // Built-in LED on some ESP-32 boards
#else
#define ONBOARDLED 2 // Built-in LED on ESP-12/ESP-07
#endif

// NTP configuration
const PROGMEM char* ntpServer = "pool.ntp.org";
const int NTP_TIMEOUT = 5000;

bool wifiFirstConnected = false;

boolean syncEventTriggered = false; // True if a time event has been triggered
NTPEvent_t ntpEvent; // Last triggered event

void onWifiEvent(WiFiEvent_t event) {
    Serial.printf("[WiFi-event] event: %d\n", event);

    switch (event) {
    case WiFiEvent_t::SYSTEM_EVENT_STA_CONNECTED:
        Serial.println("Connected to WiFi. Asking for IP address.");
        break;
    case WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP:
        Serial.printf("Got IP: %s\n", WiFi.localIP().toString().c_str());
        Serial.println("WiFi is connected.");
        digitalWrite(ONBOARDLED, LOW); // Turn on LED
        wifiFirstConnected = true;
        break;
    case WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.println("Disconnected from WiFi");
        digitalWrite(ONBOARDLED, HIGH); // Turn off LED
        WiFi.reconnect();
        break;
    default:
        break;
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println();

    WiFi.mode(WIFI_STA);
    WiFi.begin(YOUR_WIFI_SSID, YOUR_WIFI_PASSWD);

    pinMode(ONBOARDLED, OUTPUT); // Onboard LED
    digitalWrite(ONBOARDLED, HIGH); // Switch off LED

    WiFi.onEvent(onWifiEvent);
}

void loop() {
    if (wifiFirstConnected) {
        wifiFirstConnected = false;

        // Use deep sleep for 10 seconds
        esp_sleep_enable_timer_wakeup(10 * 1000000); // 10 seconds in microseconds
        esp_deep_sleep_start();
    }

    // Your code to execute when waking up from deep sleep
    Serial.println("Waking up from deep sleep...");
    delay(10000); // Print data for 10 seconds

    // Go back to deep sleep for 10 seconds
    esp_sleep_enable_timer_wakeup(10 * 1000000); // 10 seconds in microseconds
    esp_deep_sleep_start();
}
