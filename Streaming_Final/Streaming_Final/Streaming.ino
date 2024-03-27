
#include <FS.h>
#include <SPIFFS.h>
#include <DHT.h>
#include <base64.h>
#include "sonar.h"
#include "rain.h"
boolean streamFlag=false;

const char *ssid = "Dungeon 2";
const char *password = "Sns@41le21xn";
String FIREBASE_HOST = "https://farm-automation-42083-default-rtdb.firebaseio.com/";
const char *serverName = "http://espstack.com/notify.php";
String FIREBASE_AUTH = "AIzaSyA1JDJx9ffuQa55zYY0yFEws_XylxPFxAA";
#include "FirebaseESP32.h"
FirebaseData firebaseData;
#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"
int counter=0;
String rainStatus;
unsigned long premillis=0;
int tankdistance;
int reading[6];
String urlencode(String str);
String title,des;

#define DHTPIN 14          // Pin which is connected to the DHT sensor.
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float temp;
float humi;

// CAMERA_MODEL_EXP WROVER
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    21
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27

#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      19
#define Y4_GPIO_NUM      18
#define Y3_GPIO_NUM       5
#define Y2_GPIO_NUM       4
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22

String Photo2Base64()
{

  camera_fb_t *fb = NULL;

  fb = esp_camera_fb_get();

  if (!fb)
  {

    Serial.println("Camera capture failed");

    return "";
  }

  String imageFile = "data:image/jpeg;base64,";
String encrypt = base64::encode(fb->buf, fb->len);
  


  esp_camera_fb_return(fb);

//  return imageFile;
//Serial.println(encrypt);
return encrypt;
}

// https://github.com/zenmanenergy/ESP8266-Arduino-Examples/

String urlencode(String str)

{

  String encodedString = "";

  char c;

  char code0;

  char code1;

  char code2;

  for (int i = 0; i < str.length(); i++)
  {

    c = str.charAt(i);

    if (c == ' ')
    {

      encodedString += '+';
    }
    else if (isalnum(c))
    {

      encodedString += c;
    }
    else
    {

      code1 = (c & 0xf) + '0';

      if ((c & 0xf) > 9)
      {

        code1 = (c & 0xf) - 10 + 'A';
      }

      c = (c >> 4) & 0xf;

      code0 = c + '0';

      if (c > 9)
      {

        code0 = c - 10 + 'A';
      }

      code2 = '\0';

      encodedString += '%';

      encodedString += code0;

      encodedString += code1;

       encodedString+=code2;
    }

    yield();
  }

  return encodedString;
}

void setupCamera()
{
  camera_config_t config;

 config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  // init with high specs to pre-allocate larger buffers
  if (psramFound())
  {

    config.frame_size = FRAMESIZE_VGA;

    config.jpeg_quality = 12; // 0-63 lower number means higher quality

    config.fb_count = 1;
  }
  else
  {

    config.frame_size = FRAMESIZE_SVGA;

    config.jpeg_quality = 12; // 0-63 lower number means higher quality

    config.fb_count = 1;
  }
  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK)
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }
  sensor_t *s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_QQVGA); // VGA|CIF|QVGA|HQVGA|QQVGA   ( UXGA? SXGA? XGA? SVGA? )
}

void  startStreaming()
{

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.setMaxRetry(firebaseData, 3);
  Firebase.setMaxErrorQueue(firebaseData, 30);
  Firebase.enableClassicRequest(firebaseData, true);
  String jsonData = "{\"frame\":\"" + Photo2Base64() + "\"}";
  String photoPath = "/Farm";
  FirebaseJson json2;
  json2.set("frame", Photo2Base64());
  json2.set("Counter", (String)counter);
  counter++;
  if (Firebase.setJSON(firebaseData, photoPath, json2))
  {
    Serial.println(firebaseData.dataPath());
    Serial.println(firebaseData.pushName());
    Serial.println(firebaseData.dataPath() + "/" + firebaseData.pushName());
    Serial.println("Fram Uploaded");
  }
  else
  {

    Serial.println(firebaseData.errorReason());
  }
}

void readTempHumi(){
  temp=dht.readTemperature();
  humi=dht.readHumidity();
  Serial.println((String)+temp);
   Serial.println((String)+humi);

}

void uploadSensorData(){
   Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.setMaxRetry(firebaseData, 3);
  Firebase.setMaxErrorQueue(firebaseData, 30);
  Firebase.enableClassicRequest(firebaseData, true);
  //String jsonData = "{\"temp\":\"" + temp + "\",\"humi\":\"" + humi + "\"}";
  String photoPath = "/SensorData";
  FirebaseJson json2;
  json2.set("temp", temp);
  json2.set("hmi", humi);
     json2.set("tank", tankdistance);
   json2.set("rain", rainStatus);
  counter++;
  if (Firebase.setJSON(firebaseData, photoPath, json2))
  {
    Serial.println(firebaseData.dataPath());
    Serial.println(firebaseData.pushName());
    Serial.println(firebaseData.dataPath() + "/" + firebaseData.pushName());
    Serial.println("Data uploaded");
  }
  else
  {

    Serial.println(firebaseData.errorReason());
  }
}
void sendNotification()
{
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);

  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST("{\"title\":\""+rfidSt+"\",\"des\":\""+des+"\"}");

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);



  // Free resources
  http.end();
}

void setup()
{

  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  Serial.begin(115200);
  Serial.setDebugOutput(true);
 dht.begin();
 setupSonar();
 setuprain();
 WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  setupCamera();
  delay(1000);
  // drop down frame size for higher initial frame rate
}

void loop()
{
   if(millis() - premillis >5000){
     for (int j = 0; j < 6; j++) {
     reading[j] = analogRead(rain_PIN);
     delay(10);
     }
     int sensorValue = 0;
  for (int j = 0; j < 6; j++) {
    sensorValue = sensorValue + reading[j]; // add them up
  }
   sensorValue = sensorValue / 6;
  Serial.println(sensorValue);
  if(sensorValue >thold)
  {
rainStatus="Raining";
  }
  else{
    rainStatus="Not Raining";
  }
  tankdistance=getDist();
Serial.println("tankdistance  "+(String)tankdistance);
 readTempHumi();
   uploadSensorData();


//########################Logic for sending Notification ###########
  if(temp < tempT ){
   tempCounter++;
   if(tempCounter == 1){
title ="LOW TEMPERATURE"
des="Your Farm Temperature is "+temp ;
sendNotification();
   }
  }
  else{
    tempCounter=0;
  }
  //###########
    if(tankdistance < distanceT ){
   disCounter++;
   if(disCounter == 1){
title ="LOW WATER LEVEL "
des="Your Farm Water Level is "+tankdistance ;
sendNotification();
   }
  }
  else{
    disCounter=0;
  }
//########################
    if(rainStatus.indexof("Raining")>-1){
   rainCounter++;
   if(rainCounter == 1){
title ="IT IS RAINING "
des="it is Raining in Farm";
sendNotification();
   }
  }
  else{
    rainCounter=0;
  }
  //############3
  premillis = millis();
  }
  
startStreaming();


  delay(100);
}