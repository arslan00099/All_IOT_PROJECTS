#include <Arduino.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TimeLib.h>

const char *ssid = "RIGJAZZ";
const char *password = "Kashmiri786";

// Your Domain name with URL path or IP address with path
const char *serverName = "http://api.openweathermap.org/data/2.5/forecast?id=1183992&appid=d06dac366efe8af8f777825853d91bac";

unsigned long lastTime = 0;
unsigned long timerDelay = 6000;
String Readings;

String lastday;
int count=0;

String httpGETRequest(const char *serverName)
{
  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverName);
  int httpResponseCode = http.GET();
  String payload = "{}";

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
   Serial.println((String)payload);
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
 
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

      Readings = httpGETRequest(serverName);
     // Serial.println((String)sensorReadings);
      DynamicJsonDocument doc(5000);
      DynamicJsonDocument filter(5000);
   
filter["city"]["name"] = true;
      filter["list"][0]["dt"] = true;
      filter["list"][0]["main"]["temp"] = true;
      filter["list"][0]["main"]["humidity"] = true;
        filter["list"][0]["weather"][0]["main"] = true;

 

     
      deserializeJson(doc, Readings, DeserializationOption::Filter(filter));

      // Print the result
      serializeJsonPretty(doc, Serial);
      lastday="no";
      count =0;
      Serial.println("##############################################");
for(int i=0; i<100; i++){
  // Serial.println("##############  "+(String)i+ "  ###################");
   unsigned long t = doc["list"][i]["dt"];
       String city=doc["city"]["name"]; 
  
    char buff[32];
  sprintf(buff, "%02d.%02d.%02d %02d:%02d:%02d", day(t), month(t), year(t), hour(t), minute(t), second(t));
  
  String day=(dayStr(weekday(t))); 
  day.remove(3, day.length());
 // Serial.println(day);
  if(day.indexOf(lastday)> -1){}
  else{
    if(count<3){
     Serial.println("##############  "+(String)count+ "  ###################");
    int temp = doc["list"][i]["main"]["temp"];
  String weather = doc["list"][i]["weather"][0]["main"];
 

Serial.println((String)city);
 Serial.println("TEMP : " + (String)temp);
   Serial.println("EPOUCH : " + (String)t);
   Serial.println("WEATHER : " + (String)weather); 
   count++;
   lastday = day;
    }
 }

}
      // timerDelay=millis();
      delay(20 * 1000);
    }  
}}
