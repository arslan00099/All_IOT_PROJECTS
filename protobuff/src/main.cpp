#include <Arduino.h>
#include "test.pb.h"
#include <EEPROM.h> 
#include "pb_common.h"
#include "pb.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include <SPIFFS.h>


// Function to save data in Protocol Buffer format
void saveSensorData() {

    SensorData data = { .temperature = 70.0f , .humidity = 60.0f };
    uint8_t buffer[SensorData_size];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    if (!pb_encode(&stream, SensorData_fields, &data)) {
        printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
        Serial.println("ERROR");
        return;
    }
        Serial.println("ENCODED");
    // Now, you can save 'buffer' to a file or flash memory.
    // For example, using the ESP32 file system APIs or EEPROM library.
    // File saving example:
    FILE* file = fopen("/spiffs/sensor_data.bin", "w");
    fwrite(buffer, 1, stream.bytes_written, file);
    fclose(file);
}

// Function to retrieve data from Protocol Buffer format
void retrieveSensorData() {
    uint8_t buffer[SensorData_size];

    // Read 'buffer' from the file or flash memory.
    // For example, using the ESP32 file system APIs or EEPROM library.
    // File retrieval example:
    FILE* file = fopen("/spiffs/sensor_data.bin", "r");
    fread(buffer, 1, sizeof(buffer), file);
    fclose(file);

    SensorData data = { 0 }; // Initialize with zeros
    pb_istream_t stream = pb_istream_from_buffer(buffer, sizeof(buffer));

    if (!pb_decode(&stream, SensorData_fields, &data)) {
        printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
        return;
    }

    // Use the data
    printf("Temperature: %f\n", data.temperature);
    printf("Humidity: %f\n", data.humidity);
}

void listFiles(const char* dirname) {
  Serial.printf("Listing files in directory: %s\n", dirname);

  File root = SPIFFS.open(dirname);
  if (!root.isDirectory()) {
    Serial.println("Error: It's not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    String filename = file.name();
    size_t fileSize = file.size();
    Serial.printf("File: %s, Size: %u bytes\n", filename.c_str(), fileSize);
    file = root.openNextFile();
  }
}

void setup() {
  Serial.begin(115200);
   if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
listFiles("/spiffs");

 saveSensorData();
  delay(2000);
  retrieveSensorData();
 
 
}

void loop() {
  // Your main loop code goes here
}
