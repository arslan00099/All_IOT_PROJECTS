#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char *ssid = "RIGJAZZ";
const char *password = "Kashmiri786";

// Your Domain name with URL path or IP address with path
const char *serverName = "https://api.openweathermap.org/data/2.5/forecast?id=1183992&appid=d06dac366efe8af8f777825853d91bac";

unsigned long lastTime = 0;
unsigned long timerDelay = 6000;

String sensorReadings;
float sensorReadingsArr[3];

String httpGETRequest(const char *serverName)
{
  WiFiClient client;
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);


  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void setup()
{
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop()
{
  // Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay)
  {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {

      sensorReadings = httpGETRequest(serverName);
     // Serial.println((String)sensorReadings);
      DynamicJsonDocument doc(2500);
      DynamicJsonDocument filter(2000);
    
     // filter["list"][0]["dt"] = true;
      filter["dates"][0]["games"][0]["status"]["detailedState"] = true;
      filter["dates"][0]["games"][0]["teams"]["away"]["score"]= true;
      filter["dates"][0]["games"][0]["teams"]["away"]["team"]["id"] = true;
      filter["dates"][0]["games"][0]["teams"]["home"]["team"]["id"] = true;
      //  filter["list"][0]["main"]["temp"] = true;

     
      deserializeJson(doc, sensorReadings, DeserializationOption::Filter(filter));

      // Print the result
      serializeJsonPretty(doc, Serial);
      
      Serial.println("##############################################");
for(int i=0; i<11; i++){
  Serial.println("##############  "+(String)i+ "  ###################");
  String schedule = doc["dates"][0]["games"][i]["status"]["detailedState"];
  int score = doc["dates"][0]["games"][i]["teams"]["away"]["score"];
  String teamone = doc["dates"][0]["games"][i]["teams"]["away"]["team"]["id"];
  String teamtwo = doc["dates"][0]["games"][i]["teams"]["home"]["team"]["id"];
  Serial.println("Schedule : " + (String)schedule);
  Serial.println("Score : " + (String)score);
  Serial.println("teamone : " + (String)teamone);
  Serial.println("teamtwo : " + (String)teamtwo);
      
}
      // timerDelay=millis();
      delay(20 * 1000);
    }  
}}
