#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Neo";
const char* password = "12345678";

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");

  // Make HTTP POST request
  HTTPClient http;
  http.begin("http://trippler.tech/postapi.php"); // Specify the URL
  http.addHeader("Content-Type", "application/json");

  // Construct JSON payload
  String jsonPayload = "{"
                       "\"container_id\": \"7787\","
                       "\"container_count\": \"3\","
                       "\"Business_Name\": \"test\","
                       "\"oil_name\": \"oil name\","
                       "\"percentage\": \"82\","
                       "\"actual_weight\": \"90\","
                       "\"logtime\": \"29/2/2024 1:20\","
                       "\"address\": \"test address\""
                       "}";

  // Send HTTP POST request with JSON payload
  int httpResponseCode = http.POST(jsonPayload);

  // Check for response
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println("Response payload: ");
    Serial.println(payload);
  } else {
    Serial.print("Error in HTTP POST request: ");
    Serial.println(httpResponseCode);
  }

  // Cleanup
  http.end();
}

void loop() {
  // Nothing to do here
}
