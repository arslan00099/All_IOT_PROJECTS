#include <WiFi.h>
#include <HTTPClient.h>
#include <FS.h>
#include <Arduino.h>
#include <Preferences.h>
Preferences preferences;
unsigned long wifiPreMillis=0;

String ssid,password;

void loadPreferences(){
    preferences.begin("my-app", false);
    ssid=preferences.getString("ssid");
    password=preferences.getString("pass");
    if((ssid.length() > 1 ) || (password.length() > 1)){
    Serial.println("ssid : "+(String)ssid);
    Serial.println("pass : "+password);
    }
    else{
        preferences.begin("my-app", false);
preferences.putInt("rst",0); // if 1 thne post data if 0 then open AP mode
ESP.restart(); 
    }
}


void postDataToAPI(const char* filePath, const char* apiURL) {
    loadPreferences();
  // Mount the SPIFFS file system
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount SPIFFS");
    return;
  }

  // Open the file for reading
  File file = SPIFFS.open(filePath, "r");
  if (!file) {
    Serial.println("Failed to open file");
    return;
  }

  // Read the file into a String
  String jsonData = file.readString();

  // Close the file
  file.close();

  // Create an HTTPClient object
  HTTPClient http;

  // Make an HTTP POST request to the API endpoint
  http.begin(apiURL);
  http.addHeader("Content-Type", "application/json");
  int attempt = 0;
  int httpResponseCode;
int counter=0;
  do {
    // Establish WiFi connection
    WiFi.begin(ssid.c_str(), password.c_str());
    while ((WiFi.status() != WL_CONNECTED)){
          Serial.print(".");
    delay(1000);
    counter++;
    if(counter == 10){
     preferences.begin("my-app", false);
     preferences.putInt("rst",0); // if 1 thne post data if 0 then open AP mode
     Serial.println("Switching from 1 to 0");
     delay(5000);
     ESP.restart();
    }
    }
    Serial.println("WiFi connected");

    httpResponseCode = http.POST(jsonData);

    if (httpResponseCode == 200) {
      // Request succeeded
      Serial.print("HTTP response code: ");
      Serial.println(httpResponseCode);
      String response = http.getString();
      Serial.println(response);
      break;
    } else {
      // Request failed
      Serial.print("HTTP request failed. Error code: ");
      Serial.println(httpResponseCode);
      attempt++;
      if (attempt < 2) {
        // Retry after a delay
        delay(2000);
      } else {
        // Failed after two attempts, disconnect WiFi and open AP mode
        http.end();
        WiFi.disconnect();
        WiFi.softAP("ESP32-AP", "password");
        Serial.println("WiFi disconnected. AP mode activated.");
        break;
      }
    }
  } while (attempt < 2);

  // Disconnect and cleanup
  http.end();

  preferences.begin("my-app", false);
preferences.putInt("rst",0); // if 1 thne post data if 0 then open AP mode
ESP.restart();
}


void getDataFromServer() {

loadPreferences();
  // Connect to Wi-Fi
  WiFi.begin(ssid.c_str(), password.c_str());
  int counter=0;
  while ((WiFi.status() != WL_CONNECTED)) {
    Serial.print(".");
    delay(1000);
    counter++;
    if(counter == 10){
     preferences.begin("my-app", false);
     preferences.putInt("rst",0); // if 1 thne post data if 0 then open AP mode
     Serial.println("Switching from 1 to 0");
     delay(5000);
     ESP.restart();
    }
  }
  Serial.println("Connected to WiFi");

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Failed to mount SPIFFS");
    return;
  }

  // Fetch JSON data from the server
  HTTPClient http;
  http.begin("http://espstack.com/pump/fingerprintSideGetApi.php");
  int httpResponseCode = http.GET();

  if (httpResponseCode == HTTP_CODE_OK) {
    String jsonData = http.getString();
    Serial.println("DATA RESPONSE : "+(String)jsonData);
if(jsonData.length() > 5 ){
    // Save JSON data to SPIFFS file
    File jsonFile = SPIFFS.open("/data.json", "w");
    if (!jsonFile) {
      Serial.println("Failed to open file for writing");
    } else {
      jsonFile.print(jsonData);
      jsonFile.close();
      Serial.println("JSON data saved to file");
    }
}
  } else {
    Serial.print("HTTP GET request failed with error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
    preferences.begin("my-app", false);
preferences.putInt("rst",0); // if 1 thne post data if 0 then open AP mode
ESP.restart();
}
