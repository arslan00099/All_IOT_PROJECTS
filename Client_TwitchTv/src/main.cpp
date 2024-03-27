
#include <Arduino.h>
extern "C" {
#include "mbedtls/md5.h"
}
#include <WiFi.h>
#include <HTTPClient.h>
#include "Filter.h"
unsigned long preveMillCh=0;
const char *root_ca =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIGYzCCBUugAwIBAgIQAXyWwDvpf2m8ORfW4vhz7DANBgkqhkiG9w0BAQsFADBY\n"
    "MQswCQYDVQQGEwJCRTEZMBcGA1UEChMQR2xvYmFsU2lnbiBudi1zYTEuMCwGA1UE\n"
    "AxMlR2xvYmFsU2lnbiBBdGxhcyBSMyBEViBUTFMgQ0EgMjAyMiBRNDAeFw0yMjEx\n"
    "MjgyMTI1MDRaFw0yMzEyMzAyMTI1MDNaMBQxEjAQBgNVBAMMCXR3aXRjaC50djCC\n"
    "ASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALw4a784/pqlLE35clbz9lLm\n"
    "2r6rEnN+XuzQeSgER6xwBTaa5P2T7r2V/4PwFgGcYWLtjmEzB34EnoV/GccorzYK\n"
    "t3mXryii5R4XA9o0Sr9Ep0GVn9CVvTmbSbpHqNZIJuW5tmBuCP6c0rVrnsfh/D20\n"
    "D3CrhMv4t9bEjY6Np+K/58DBgsMGkxPo1i18lhmJSYtL4EwhTjRWPHOOUdMUS0cp\n"
    "SngpOw2vwUMsPWHtKMKV5Vo/VI6XrNvjwUI4fiPdRPJ4RabYG4BDaQRnBL2mLafp\n"
    "/UJSPpJUH+rD5RpuD899gFtdkIWbAUFbYFHxYRu6xpNC3KdKfFVH8rcK3JLTEp0C\n"
    "AwEAAaOCA2swggNnMCEGA1UdEQQaMBiCCXR3aXRjaC50doILKi50d2l0Y2gudHYw\n"
    "DgYDVR0PAQH/BAQDAgWgMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjAd\n"
    "BgNVHQ4EFgQU1QrZcLgEufIby+hSyydnWILUs6owVwYDVR0gBFAwTjAIBgZngQwB\n"
    "AgEwQgYKKwYBBAGgMgoBAzA0MDIGCCsGAQUFBwIBFiZodHRwczovL3d3dy5nbG9i\n"
    "YWxzaWduLmNvbS9yZXBvc2l0b3J5LzAMBgNVHRMBAf8EAjAAMIGeBggrBgEFBQcB\n"
    "AQSBkTCBjjBABggrBgEFBQcwAYY0aHR0cDovL29jc3AuZ2xvYmFsc2lnbi5jb20v\n"
    "Y2EvZ3NhdGxhc3IzZHZ0bHNjYTIwMjJxNDBKBggrBgEFBQcwAoY+aHR0cDovL3Nl\n"
    "Y3VyZS5nbG9iYWxzaWduLmNvbS9jYWNlcnQvZ3NhdGxhc3IzZHZ0bHNjYTIwMjJx\n"
    "NC5jcnQwHwYDVR0jBBgwFoAUiGjZHxHEDh+bieCgQndfw3mOw24wSAYDVR0fBEEw\n"
    "PzA9oDugOYY3aHR0cDovL2NybC5nbG9iYWxzaWduLmNvbS9jYS9nc2F0bGFzcjNk\n"
    "dnRsc2NhMjAyMnE0LmNybDCCAX8GCisGAQQB1nkCBAIEggFvBIIBawFpAHcAb1N2\n"
    "rDHwMRnYmQCkURX/dxUcEdkCwQApBo2yCJo32RMAAAGEwCEVKAAABAMASDBGAiEA\n"
    "18sWfnyL4efZnLkJNVPYu5IBwI+URyq0Pq5cLBcPxhcCIQCY5hZaCj5BFOh2neFW\n"
    "Q0TH+9nsck3BxmxMMCVT89c6bQB2AK33vvp8/xDIi509nB4+GGq0Zyldz7EMJMqF\n"
    "hjTr3IKKAAABhMAhFR8AAAQDAEcwRQIgZIb+D8KKRQRLwQ4Np7xm7/5aDEqW8X54\n"
    "7taBubtf9R8CIQCnvp4XF0W3bA37vfpV8/h9PD6tHAuIS1kn58/lKGf6DwB2AFWB\n"
    "1MIWkDYBSuoLm1c8U/DA5Dh4cCUIFy+jqh0HE9MMAAABhMAhFZIAAAQDAEcwRQIg\n"
    "U2tYM55usO/qGEcOAJ61eh68eT1dcgOHVgkQLDaBpAkCIQDGQgbLDxGBc/txvevS\n"
    "OOCnLWY0b/W/sBbmLjtq5FP97TANBgkqhkiG9w0BAQsFAAOCAQEASfTCE6Gocq7a\n"
    "uPsxbjqOVBo0x/82c+9+eW3n1xv7og8hLKegR2GqEAwx07+vM+ewAVak3R1QsVxL\n"
    "1GG/xDCSPTgVi0Dd50Ckv4JIDNIjS2B+KgafskrDOZGIJ+q+jvriaahinFdztWIr\n"
    "KtFgwRsq0GXE3dHCvhGvnYVv8Obx5cYNmtY7cJgmjMkeCPlGupR0RAGSierjrMBN\n"
    "WMvB9SIJrm+BahIzMVCZ1ce0U3Wxahg23U+ytSkm29blhyZy0j2jCbmLeGQ0weZr\n"
    "LdeXSvVk5oKcZElGThfIt/3lcgQZ6Wqg480lY5iY3o+R3LKAHcPwcHLQIigyDqiH\n"
    "zvEjdUwTKQ==\n"
    "-----END CERTIFICATE-----\n";

const char *ssid = "RIGJAZZ";
const char *password = "Kashmiri786";
void getOAuthT()
{
  if ((WiFi.status() == WL_CONNECTED))
  { // Check the current connection status

    HTTPClient http;

    http.begin("https://id.twitch.tv/oauth2/token"); // Specify the URL and certificate
    // http.addHeader("Content-Type", "client_credentials");
    //  http.addHeader("Client-Id", "i494ud37781vkq749zeqq1fmivdr5m");
    // http.addHeader("client_secret", "mvpfihvl52kxpwvw991uiw5yq4h0p2");
    //   http.addHeader("grant_type", "client_credentials");

    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpCode = http.POST("client_id="+user_id+"&client_secret="+user_secret+"&grant_type=client_credentials"); // Make the request
    String payload;
    if (httpCode > 0)
    { // Check for the returning code

      payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
    }

    else
    {
      Serial.println("Error on HTTP request");
    }
    StaticJsonDocument<200> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, payload);
    if (error)
      Serial.println(F("Failed to read file, using default configuration"));

    // Copy values from the JsonDocument to the Config
    String access_token = doc["access_token"];
    Serial.println("access_token: " + access_token);
      preferences.begin("my-app", false);
    preferences.putString("accesstoken", access_token);
    preferences.end();
  }
}

void getChanelSt()
{

  if ((WiFi.status() == WL_CONNECTED))
  { // Check the current connection status

    HTTPClient http;

    http.begin("https://api.twitch.tv/helix/search/channels?query="+channelName+"&live_only=false"); // Specify the URL and certificate
    http.addHeader("Authorization", "Bearer "+access_token);
    http.addHeader("Client-Id", user_id.c_str());

    int httpCode = http.GET(); // Make the request

    if (httpCode > 0)
    { // Check for the returning code

      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
      filterData(payload);
    }

    else
    {
      Serial.println("Error on HTTP request");
    }

    http.end(); // Free the resources
  }

  delay(10000);
}

void setup()
{

  Serial.begin(115200);
  pinMode(0,INPUT);
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);
  delay(1000);
  setupWifiManger();
  // WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
 // getOAuthT();
  setupWifiM();
 getpreferences();
 getChanelSt();

}

void loop()
{
  if(digitalRead(0)==LOW)
  {
    wm.resetSettings();
    ESP.restart();
  }
  if(millis()-preveMillCh>15000)
  {
    getChanelSt();
    preveMillCh=millis();
  }
}