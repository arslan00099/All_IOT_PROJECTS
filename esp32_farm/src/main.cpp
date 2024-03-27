
// INCLUDES
#include <Arduino.h> // General functionality
#include <esp_camera.h> // Camera
#include <SPIFFS.h>
#include "SPI.h"
#include "driver/rtc_io.h"
#include "FS.h" // File System
#include "soc/soc.h" // System settings (e.g. brownout)
#include "soc/rtc_cntl_reg.h"
#include "driver/rtc_io.h"
#include <EEPROM.h> // EEPROM flash memory
#include <WiFi.h> // WiFi
#include "time.h" // Time functions
#include "ESP32_MailClient.h"

#include <WiFiUdp.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
WiFiServer server(80);


#define DHTPIN 14          // Pin which is connected to the DHT sensor.
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
char auth[] = "HjG68BvqNSvyntYnFyGr1LSdofdRdbtN";
// DEFINES
//#define USE_INCREMENTAL_FILE_NUMBERING //Uses EEPROM to store latest file stored
//#define USE_TIMESTAMP // Uses Wi-Fi to retrieve current time value
#define SEND_EMAIL // Uses Wi-Fi to email photo attachment
#define CAMERA_MODEL_WROVER_KIT
//#define TRIGGER_MODE // Photo capture triggered by GPIO pin rising/falling
//#define TIMED_MODE // Photo capture automated according to regular delay

// Wi-Fi settings
#define WIFI_SSID "Dungeon 2"
#define WIFI_PASSWORD "Sns@41le21xn"
#define sender_email    "gtsaravanan2@live.com"
#define sender_email_password   ""
#define SMTP_Server            "smtp.office365.com"
#define SMTP_Server_Port        587
#define email_subject          "ESP32-CAM Image Capture"
#define email_recipient        "saravanangt777@gmail.com"

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


// Set GPIOs for LED and PIR Motion Sensor
const int led = 2;
const int motionSensor = 13;
SMTPData smtpData;

#define IMAGE_PATH "/image.jpg"

void IRAM_ATTR detectsMovement() {
  Serial.println("MOTION DETECTED!!!");
  digitalWrite(led, HIGH);
}

void setup() {
  

  // CAUTION - We'll disable the brownout detection
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  // Start serial connection for debugging purposes
  Serial.begin(115200);
 dht.begin();
  //Blynk.begin(auth, WIFI_SSID, WIFI_PASSWORD);
  Blynk.begin(auth, WIFI_SSID, WIFI_PASSWORD, "blynk.cloud", 80);
  // Pin definition for CAMERA_MODEL_AI_THINKER
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

  // If the board has additional "pseudo RAM", we can create larger images
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA; // UXGA=1600x1200. Alternative values: FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
   pinMode(motionSensor, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);


  // Initialise the camera
  // Short pause helps to ensure the I2C interface has initialised properly before attempting to detect the camera
  delay(250);
  if (!SPIFFS.begin(true)) {
    Serial.println("An error occurred while mounting the file system");
}

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
  
  }

  // Image settings
  sensor_t * s = esp_camera_sensor_get();
  s->set_gain_ctrl(s, 1);      // Auto-Gain Control 0 = disable , 1 = enable
  s->set_agc_gain(s, 0);       // Manual Gain 0 to 30
  s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
  // Exposure
  s->set_exposure_ctrl(s, 1);  // Auto-Exposure Control 0 = disable , 1 = enable
  s->set_aec_value(s, 300);    // Manual Exposure 0 to 1200
  // Exposure Correction
  s->set_aec2(s, 0);           // Automatic Exposure Correction 0 = disable , 1 = enable
  s->set_ae_level(s, 0);       // Manual Exposure Correction -2 to 2
  // White Balance
  s->set_awb_gain(s, 1);       // Auto White Balance 0 = disable , 1 = enable
  s->set_wb_mode(s, 0);        // White Balance Mode 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
  s->set_whitebal(s, 1);       // White Balance 0 = disable , 1 = enable
  s->set_bpc(s, 0);            // Black Pixel Correction 0 = disable , 1 = enable
  s->set_wpc(s, 1);            // White Pixel Correction 0 = disable , 1 = enable
  s->set_brightness(s, 2);     // Brightness -2 to 2
  s->set_contrast(s, 1);       // Contrast -2 to 2
  s->set_saturation(s, 0);     // Saturation -2 to 2
  s->set_special_effect(s, 0); // (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
  // Additional settings
  s->set_lenc(s, 1);           // Lens correction 0 = disable , 1 = enable
  s->set_hmirror(s, 0);        // Horizontal flip image 0 = disable , 1 = enable
  s->set_vflip(s, 0);          // Vertical flip image 0 = disable , 1 = enable
  s->set_colorbar(s, 0);       // Colour Testbar 0 = disable , 1 = enable
  s->set_raw_gma(s, 1);        // 0 = disable , 1 = enable
  s->set_dcw(s, 1); 
  
   
  camera_fb_t *fb = NULL;
  
  fb = esp_camera_fb_get();
  // Check it was captured ok  
  if(!fb) {
    Serial.println("Camera capture failed");
  
  }
  Serial.printf("Picture file name is %s\n", IMAGE_PATH);
    File file = SPIFFS.open(IMAGE_PATH, FILE_WRITE);
    if (!file) {
      Serial.println("Failed to open file in writing mode");
    }
    else {
      file.write(fb->buf, fb->len); // payload (image), payload length
      Serial.print("The picture has been saved in ");
      Serial.print(IMAGE_PATH);
      Serial.print(" - Size: ");
      Serial.print(file.size());
      Serial.println(" bytes");
    }
    file.close();
    esp_camera_fb_return(fb);

  // Turn flash off after taking picture
  //ledcWrite(7, 0);

  // Build up the string of the filename we'll use to save the file
 
  // Connect to Wi-Fi if required
  #if defined(SEND_EMAIL) 
    WiFi.mode(WIFI_STA);
    WiFi.setHostname("BirdFeederCam");
    int connAttempts = 0;
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED && connAttempts < 10) {
      Serial.print(".");
      delay(500);
      connAttempts++;
    }
    if(WiFi.isConnected()){
      Serial.println("");
      Serial.println("WiFi connected.");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      Serial.print(" Signal Level: ");
      Serial.println(WiFi.RSSI());
      Serial.println();
    }
    else {
      Serial.println(F("Failed to connect to Wi-Fi"));
      
    }
  #endif
 server.begin();
  Serial.println("Server started");
  Serial.print("Camera Stream Ready! Go to: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/stream");
  sendImage();

  // And go to bed until the next time we are triggered to take a photo
} 
 
void loop() {
  sens();
  str();
   int pirState = digitalRead(motionSensor); // Read the PIR sensor state
  Serial.println(pirState); // Print the state to the serial monitor for debugging

  if (pirState == HIGH) { // If motion is detected
    sendImage();
  } 
Blynk.run();
//delay(100); 
}
void sendImage( void ) {
  smtpData.setLogin(SMTP_Server, SMTP_Server_Port, sender_email, sender_email_password);
  smtpData.setSender("ESP32-CAM", sender_email);
  smtpData.setPriority("High");
  smtpData.setSubject(email_subject);
  smtpData.setMessage("PFA ESP32-CAM Captured Image.", false);
  smtpData.addRecipient(email_recipient);
  smtpData.addAttachFile(IMAGE_PATH, "image/jpg");
  smtpData.setFileStorageType(MailClientStorageType::SPIFFS);
   smtpData.setSendCallback(sendCallback);
  
  
  if (!MailClient.sendMail(smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());

  smtpData.empty();
}
void sendCallback(SendStatus msg) {
  Serial.println(msg.info());
}
void sens() {
  //delay(200);
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\t");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
 }
void str() {
;
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  String response;
  response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: multipart";
  response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
client.print(response);

while (1) {
camera_fb_t *fb = esp_camera_fb_get();
if (!fb) {
Serial.println("Camera capture failed");
break;
}
client.print("--frame\r\n");
client.print("Content-Type: image/jpeg\r\n");
client.print("Content-Length: " + String(fb->len) + "\r\n");
client.print("\r\n");
client.write(fb->buf, fb->len);
client.print("\r\n");

esp_camera_fb_return(fb);

delay(1);
yield();
}
}

BLYNK_WRITE(V3) {
int pinValue = param.asInt();
if (pinValue == 1) {
Serial.println("Button pressed!");
}
}
