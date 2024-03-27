#include <Arduino.h>
#include "test.pb.h"
#include <EEPROM.h> 
#include "pb_common.h"
#include "pb.h"
#include "pb_encode.h"
#include "pb_decode.h"

// Buffer size to store serialized data
#define BUFFER_SIZE 30
#define EEPROM_SIZE 512

// Function to store serialized data in non-volatile memory (you may implement this as needed)
void store_data_in_nvm(const uint8_t* data, size_t size) {
  if (size > EEPROM_SIZE) {
    Serial.println("Data size exceeds EEPROM capacity");
    return; // Handle the error accordingly
  }

  // Write the serialized data to EEPROM
  for (size_t i = 0; i < size; i++) {
    EEPROM.write(i, data[i]);
    Serial.println("ID "+(String)i+"------"+(String)data[i]);
  }
  EEPROM.commit(); // Commit the changes to EEPROM
}


// Function to retrieve serialized data from non-volatile memory (you may implement this as needed)
void retrieve_data_from_nvm(uint8_t* data, size_t size) {
  if (size > EEPROM_SIZE) {
    Serial.println("Buffer size exceeds EEPROM capacity");
    return; // Handle the error accordingly
  }

  // Read the serialized data from EEPROM
  for (size_t i = 0; i < size; i++) {
    data[i] = EEPROM.read(i);
      Serial.println("ID "+(String)i+"------"+(String)data[i]);
  }
}

void store_sensor_data(float temperature, float humidity, int32_t pressure) {
  // Create a SensorData message
  SensorData sensor_data = SensorData_init_zero;
  sensor_data.temperature = temperature;
  sensor_data.humidity = humidity;
  sensor_data.pressure = pressure;

  // Serialize the message into a buffer
  uint8_t buffer[BUFFER_SIZE];
  pb_ostream_t stream = pb_ostream_from_buffer(buffer, 30);
  bool status = pb_encode(&stream, SensorData_fields, &sensor_data);

  if (status) {
    Serial.println("SAVED");
    // Store the serialized data in non-volatile memory
    store_data_in_nvm(buffer, stream.bytes_written);
  } else {
    Serial.println("UNSAVED");
    // Failed to encode the message
    // Handle the error
  }
}

void fetch_sensor_data() {
  // Buffer to hold the retrieved serialized data
  uint8_t buffer[BUFFER_SIZE];

  // Retrieve the serialized data from non-volatile memory
  retrieve_data_from_nvm(buffer, BUFFER_SIZE);

  // Deserialize the data from the buffer back into a SensorData message
  SensorData sensor_data = SensorData_init_zero;
  pb_istream_t stream = pb_istream_from_buffer(buffer, 30);
  bool status = pb_decode(&stream, SensorData_fields, &sensor_data);

  if (status) {
    // Successfully decoded the data
    // Now you can use the data stored in the sensor_data variable
    float temperature = sensor_data.temperature;
    float humidity = sensor_data.humidity;
    int32_t pressure = sensor_data.pressure;

    // Do something with the fetched data, for example, print it
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("°C, Humidity: ");
    Serial.print(humidity);
    Serial.print("%, Pressure: ");
    Serial.print(pressure);
    Serial.println(" hPa");
  } else {
    // Failed to decode the message
    // Handle the error
    Serial.print("ERROR : ");
    Serial.println(PB_GET_ERROR(&stream));
    Serial.println("Failed to fetch data from Protocol Buffer.");
  }
}

// Example usage
void setup() {
  Serial.begin(115200);

  float temperature = 25.5;
  float humidity = 60.0;
  int32_t pressure = 1013;
  Serial.println("CALLING");
  store_sensor_data(temperature, humidity, pressure);
  // Delay for some time to simulate other tasks
  delay(2000);
  fetch_sensor_data();
}

void loop() {
  // Your main loop code goes here
}
