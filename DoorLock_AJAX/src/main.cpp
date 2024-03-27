#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

void handleButton() {
  // process the button click here
  // ...
  
  // send the response back as plain text
  server.send(200, "text/plain", "Button clicked");
}

void setup() {
  Serial.begin(115200);
  
  WiFi.begin("Test", "12345678");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  server.on("/button", HTTP_GET, handleButton);

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}
