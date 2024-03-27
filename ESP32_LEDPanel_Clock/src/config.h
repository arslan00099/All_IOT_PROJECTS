
#include <Arduino_Json.h>
#include <WiFi.h>
#include <HTTPClient.h>
String selectionArr[22];


void getConfig(String mac) {
  String serverName = "http://iotprojectarslan.000webhostapp.com/coins/getapi.php?mac=";
 serverName.concat(mac);
  // Initialize Serial port
  delay(1000);
  Serial.println(serverName);
  Serial.println(("Connecting. to server.."+serverName));
  // Connect to HTTP server
      HTTPClient http;
 http.begin(serverName.c_str());
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);

payload= payload.substring(1,payload.length());
//payload=payload.substring(payload.indexOf('[')-1,payload.length());

Serial.println("Payload correct: "+payload);
 JSONVar myObject = JSON.parse(payload);

// JSON.typeof(jsonVar) can be used to get the type of the var
if (JSON.typeof(myObject) == "undefined") {
  Serial.println("Parsing input failed!");
  return;
}

Serial.print("JSON object = ");
Serial.println(myObject);

// myObject.keys() can be used to get an array of all the keys in the object
JSONVar keys = myObject.keys();

//for (int i = 0; i < keys.length()-1; i++) {
  for (int i = 0; i < 22; i++) {
  JSONVar value = myObject[keys[i]];
  Serial.print((String)i+": ");
  Serial.print(keys[i]);
  Serial.print(" = ");
  Serial.println(value);
  selectionArr[i] = value;
//selectionArr[i].toLowerCase();
}


 
Serial.print("2 = ");
Serial.println(selectionArr[2]);
Serial.print("3 = ");
Serial.println(selectionArr[3]);
Serial.print("4 = ");
Serial.println(selectionArr[4]);

Serial.print("12 = ");
Serial.println(selectionArr[12]);
Serial.print("13 = ");
Serial.println(selectionArr[13]);
Serial.print("14 = ");
Serial.println(selectionArr[14]);
  
    }
  

}
