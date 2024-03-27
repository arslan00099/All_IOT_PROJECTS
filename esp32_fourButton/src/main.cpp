#include <Arduino.h>
#include "index.h"
#include "HX711.h"
#include <WiFi.h>
#include <Arduino_JSON.h>
#include <EEPROM.h>
#include <ESPAsyncWebSrv.h>
int  tar;
int previousUpdateTime=0;
// Define HX711 objects for each load cell
HX711 scale_peso;
HX711 scale_lat;
HX711 scale_as;
HX711 scale_ad;
// Define the known loads for calibration
const float  knownWeight = pesoMorto;  // Use long instead of float
const float known_H_dx_sx_load = pesoMorto_sxdx;  // Use long instead of float
const float known_av_post_load =pesoMorto_av;  // Use long instead of float
String getSensorReadings();

struct SensorValues {
    float peso;
    float lat;
    float avsx;
    float avdx;
};
SensorValues sensorData; // Structure to store sensor values
boolean sendFlag=false;
SensorValues read_frame(); // Correct function declaration
JSONVar readings; //json varibale for sending reading
const char *PARAM_INPUT_1 = "output";
const char *PARAM_INPUT_2 = "state";

AsyncWebServer server(80);


String getSensorReadings() {
    if (sendFlag) {
        // Call your read_frame() function to retrieve sensor values
        sensorData = read_frame();

        readings["peso"] = String(sensorData.peso);

        // Check the second sensor value for Left or Right
        if (sensorData.lat >= 0) {
            readings["latsx"] = String("0");
            readings["latdx"] = String(sensorData.lat);
        } else {
            readings["latsx"] = String(-sensorData.lat);
            readings["latdx"] = String("0");
        }

        // Calculate the sum of the third and fourth sensors for Up or Down
        int sumThirdFourth = sensorData.avsx + sensorData.avdx;
       
       // Serial.println(sumThirdFourth);
        if (sumThirdFourth >= 0) {
            readings["retr"] = String(sumThirdFourth);
            readings["av"] = String("0");
        } else {
            readings["retr"] = String("0");
            readings["av"] = String(-sumThirdFourth);
            
        }
         Serial.println("compiling values");
    } else {
        readings["peso"] = String("0");
        readings["latsx"] = String("0");
        readings["latdx"] = String("0");
        readings["av"] = String("0");
        readings["retr"] = String("0");
    }

    String jsonString = JSON.stringify(readings);
    Serial.println(jsonString);
    return jsonString;
}


void setup() {
    Serial.begin(115200);
         // Initialize HX711 objects for each load cell
    scale_peso.begin(dtPin_peso, sckPin_peso);
    scale_lat.begin(dtPin_lat, sckPin_lat);
    scale_as.begin(dtPin_as, sckPin_as);
    scale_ad.begin(dtPin_ad, sckPin_ad);
       // Tare the scale to remove any initial weight
   scale_peso.tare();
   scale_lat.tare();
   scale_as.tare();
   scale_ad.tare();
   //define default values for scales
   scale_peso.set_scale(10);
  scale_lat.set_scale(90);
  scale_as.set_scale(71);
  scale_ad.set_scale(71);
   
    WiFi.begin(ssid, password);
    delay(1);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
     Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", htmlContent);
    });
      server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              Serial.println("Call from index file");
    String json = getSensorReadings();
    request->send(200, "application/json", json);
    json = String(); });

    server.on("/DATA", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String inputMessage1;
              String inputMessage2;

              // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
              if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2))
              {
                inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
                inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
                if (inputMessage1.indexOf("start") > -1)
                {
                 sendFlag=true;
                 Serial.println("SEND FLAG TRUE");
                }
                 if (inputMessage1.indexOf("stop") > -1)
                {
                 sendFlag=false;
                  Serial.println("SEND FLAG FALSE");
                }
              }
              else
              {
                inputMessage1 = "No message sent";
                inputMessage2 = "No message sent";
              }
              Serial.print("KEY: ");


              Serial.print(inputMessage1);
              Serial.print(" Data: ");
              Serial.println(inputMessage2);
              request->send(200, "text/plain", "OK");
           
            });


    server.begin();
}

void loop() {
   // Check if it's time to update sensor values
   // unsigned long currentMillis = millis();
  // if (currentMillis - previousUpdateTime >= updateInterval) {
     // Update sensor values
     sensorData = read_frame();
      delay(200);
    // previousUpdateTime = currentMillis; // Save the last update time
  
 //}
}
SensorValues read_frame() {
    SensorValues result;

   // Start conversions for all four HX711 load cells
    scale_peso.set_scale();  // Use default scale factor
    scale_lat.set_scale();   // Use default scale factor
    scale_as.set_scale();    // Use default scale factor
    scale_ad.set_scale();    // Use default scale factor

    // Wait for all load cells to be ready
    while (!scale_peso.is_ready() || !scale_lat.is_ready() || !scale_as.is_ready() || !scale_ad.is_ready()) {
        // Wait until all are ready
        delay(1);
    }
    
    // Read the values from all four load cells without restarting conversions
    result.peso = scale_peso.get_units(1);  // Read the weight
    result.lat = scale_lat.get_units(1);    // Read lateral
    result.avsx = scale_as.get_units(1);    // Read forward-sx
    result.avdx = scale_ad.get_units(1);    // Read forward-dx

   
    return result;
}
