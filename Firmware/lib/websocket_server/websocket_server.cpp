#include "websocket_server.h"

AsyncWebSocket ws("/ws");
AsyncWebSocketClient *client = nullptr;

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    // A client has connected
    Serial.println("WebSocket client connected");
    client->text("Connected to WebSocket server");
  } else if (type == WS_EVT_DISCONNECT) {
    // A client has disconnected
    Serial.println("WebSocket client disconnected");
  } else if (type == WS_EVT_DATA) {
    // Data received from a client
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
      // Handle text data
      data[len] = '\0';  // Add null terminator to convert to string
      String message((char*)data);
      Serial.println("Received message: " + message);

      // Send response to the client
      client->text("Server received your message: " + message);
    }
  }
}

void startWebSocketServer() {
  // Start WebSocket server
  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);
}
 
