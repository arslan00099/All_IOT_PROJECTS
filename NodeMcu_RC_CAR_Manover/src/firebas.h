
#include <Arduino.h>
void moveLeft();
void moveRight();
void moveLeftRound();
void moveRightRound();
void moveForward();
void moveForward();
void moveLeft();
void moveLeft();
void stop();
void moveBack();
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>


#include <addons/TokenHelper.h>

#include <addons/RTDBHelper.h>

#define WIFI_SSID "RIGJAZZ"
#define WIFI_PASSWORD "Kashmiri786"

#define API_KEY "AIzaSyA5s34ziPeWOkwrgLYCMY0lKItQeHkawmA"

#define DATABASE_URL "https://rccar-7c5a4-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define USER_EMAIL "itsfabulous2058@gmail.com"
#define USER_PASSWORD "#Client00998"

int pulseVal = 128;
FirebaseData stream;
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

int count = 0;

volatile bool dataChanged = false;

void streamCallback(FirebaseStream data)
{
  Serial.printf("sream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
                data.streamPath().c_str(),
                data.dataPath().c_str(),
                data.dataType().c_str(),
                data.eventType().c_str());

  printResult(data); // see addons/RTDBHelper.h
 Serial.println(data.dataPath());
Serial.println(data.boolData());
Serial.println(data.intData());

Serial.println(data.stringData());


      Serial.printf("Received stream payload size: %d (Max. %d)\n\n", data.payloadLength(), data.maxPayloadLength());
  dataChanged = true;
if(data.dataPath().indexOf("up")>-1 && data.stringData().indexOf("1")>-1)
{
  Serial.println("Moving Forward");
  stop();
  moveForward();
}

else if(data.dataPath().indexOf("down")>-1 && data.stringData().indexOf("1")>-1)
{
  Serial.println("Moving Backward");
  stop();
  moveBack();
}

else if(data.dataPath().indexOf("left")>-1 && data.stringData().indexOf("1")>-1)
{
  Serial.println("Moving left");
  stop();
  moveLeft();
}

else if(data.dataPath().indexOf("right")>-1 && data.stringData().indexOf("1")>-1)
{
  Serial.println("Moving Right");
  stop();
  moveRight();
}
else if(data.dataPath().indexOf("speed")>-1 )
{
  pulseVal=data.stringData().toInt();
}

else
{
  stop();
}
}

void streamTimeoutCallback(bool timeout)
{
  if (timeout)
    Serial.println("stream timed out, resuming...\n");

  if (!stream.httpConnected())
    Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
}

void setupFB()
{

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  config.api_key = API_KEY;


  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  config.database_url = DATABASE_URL;

  config.token_status_callback = tokenStatusCallback; 


  Firebase.begin(&config, &auth);

  Firebase.reconnectWiFi(true);

#if defined(ESP8266)
  stream.setBSSLBufferSize(2048 /* Rx in bytes, 512 - 16384 */, 512 /* Tx in bytes, 512 - 16384 */);
#endif

  if (!Firebase.RTDB.beginStream(&stream, "/test"))
    Serial.printf("sream begin error, %s\n\n", stream.errorReason().c_str());

  Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);

  /** Timeout options, below is default config.

  //WiFi reconnect timeout (interval) in ms (10 sec - 5 min) when WiFi disconnected.
  config.timeout.wifiReconnect = 10 * 1000;

  //Socket begin connection timeout (ESP32) or data transfer timeout (ESP8266) in ms (1 sec - 1 min).
  config.timeout.socketConnection = 30 * 1000;

  //ESP32 SSL handshake in ms (1 sec - 2 min). This option doesn't allow in ESP8266 core library.
  config.timeout.sslHandshake = 2 * 60 * 1000;

  //Server response read timeout in ms (1 sec - 1 min).
  config.timeout.serverResponse = 10 * 1000;

  //RTDB Stream keep-alive timeout in ms (20 sec - 2 min) when no server's keep-alive event data received.
  config.timeout.rtdbKeepAlive = 45 * 1000;

  //RTDB Stream reconnect timeout (interval) in ms (1 sec - 1 min) when RTDB Stream closed and want to resume.
  config.timeout.rtdbStreamReconnect = 1 * 1000;

  //RTDB Stream error notification timeout (interval) in ms (3 sec - 30 sec). It determines how often the readStream
  //will return false (error) when it called repeatedly in loop.
  config.timeout.rtdbStreamError = 3 * 1000;

  */
}

void loopFB()
{
  if (dataChanged)
  {
    dataChanged = false;
     }
}