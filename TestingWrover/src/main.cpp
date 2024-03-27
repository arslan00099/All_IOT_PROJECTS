
#include <SPIFFS.h>
#include "SPI.h"

#include "FS.h" // File System

#include <EEPROM.h> // EEPROM flash memory

#include "ESP32_MailClient.h"


#include <DHT.h>




#define DHTPIN 14          // Pin which is connected to the DHT sensor.
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


#define sender_email    "gtsaravanan2@live.com"
#define sender_email_password   ""
#define SMTP_Server            "smtp.office365.com"
#define SMTP_Server_Port        587
#define email_subject          "ESP32-CAM Image Capture"
#define email_recipient        "saravanangt777@gmail.com"


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
 dht.begin();
  //Blynk.begin(auth, WIFI_SSID, WIFI_PASSWORD);
   pinMode(motionSensor, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

  if (!SPIFFS.begin(true)) {
    Serial.println("An error occurred while mounting the file system");
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
//Blynk.run();
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
