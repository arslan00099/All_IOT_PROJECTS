#include <Arduino.h>
#include <ArduinoJson.h>
#include "SPIFFS.h"

void saveData() {
  const char* rfids[] = {
    "83188795211301", "83189795211301", "83190795211301", "83191795211301",
    "83180795211301", "83181795211301", "83182795211301", "83183795211301",
    "83172795211301", "83173795211301", "83174795211301"
  };

    const char* cat[] = {
    "coldrink", "snaks", "snaks", "piza",
    "candy", "WATER", "CHOCO", "coldrink",
    "JELLY", "snaks", "candy"
  };

  const char* productNames[] = {
    "PEPSI", "PUFFS", "CHIPS", "PIZAA",
    "TOAST", "WATER", "CHOCO", "FANTA",
    "JELLY", "FRIES", "PEPSI"
  };



  float prices[] = {
    160, 20, 50, 100, 30, 40, 20, 40, 10, 30, 160
  };

  int quantities[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
  };

  // Create a JSON document
  const size_t capacity = JSON_ARRAY_SIZE(11) + 11 * JSON_OBJECT_SIZE(4);
  DynamicJsonDocument doc(2000);

  // Fill the JSON document with RFID data
  JsonArray data = doc.to<JsonArray>();
  for (int i = 0; i < 11; i++) {
    JsonObject item = data.createNestedObject();
    item[rfids[i]]["cat"] = cat[i];
    item[rfids[i]]["productName"] = productNames[i];
    item[rfids[i]]["price"] = prices[i];
    item[rfids[i]]["quantity"] = quantities[i];
    item[rfids[i]]["netPrice"] = prices[i] * quantities[i];
  }

  // Serialize the JSON document to a string
  String jsonStr;
  serializeJson(doc, jsonStr);

  // Print the JSON string for debugging
  Serial.println(jsonStr);

  // Save the JSON string to SPIFFS
  File file = SPIFFS.open("/data.json", "w");
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  file.print(jsonStr);
  file.close();

  Serial.println("JSON data saved to SPIFFS");
}

void readSpiff(){
     // Read the JSON data from SPIFFS
  File file = SPIFFS.open("/data.json", "r"); // Replace with the actual file path
  if (!file) {
    Serial.println("Failed to open file");
    return;
  }

  // Parse and print the JSON data
  DynamicJsonDocument doc(2000); // Adjust the buffer size as needed
  DeserializationError error = deserializeJson(doc, file);

  if (error) {
    Serial.print("JSON parsing failed: ");
    Serial.println(error.c_str());
    return;
  }

  serializeJsonPretty(doc, Serial); // Print the JSON data to the serial monitor in a human-readable format

  // Close the file
  file.close();
}

