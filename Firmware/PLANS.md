

### Future Implementations


## Web API

The firmware includes a web API with endpoints to interact with the `settings.bin` file. These endpoints allow communication in JSON format with a web client for viewing and updating settings.

The following API endpoints are available:

- Update Settings: API endpoints to update various settings.

- Manage Melodies: API endpoints to create, update, and manage melodies, which are sequences of tones.

- Update Current Time: Functionality to sync the device's current time with an NTP server.

## ESP-NOW Protocol

The firmware supports the ESP-NOW protocol for communication between devices. It defines specific messages using the Protobuf format.

### Message Structure

The firmware uses the following Protobuf message structure for ESP-NOW messages:

```protobuf
syntax = "proto3";
package esp_now;

// Enumeration for commands
enum CommandCode {
    PING = 1;
    PONG = 2;
    PLAY_TONE = 3;
    PLAY_TONE_OK = 4;
    PLAY_TX = 5;
    PLAY_TX_OK = 6;
    SYNC_AP = 7;
    SYNC_AP_OK = 8;
    REBOOT = 7 [default = PING];
}

message esp_now_msg {
    required string target_SSID;
    required string source_SSID;
    required string source_MAC;
    required CommandCode command_code;
    optional string payload;
    required int64 timestamp;
    required int32 hops;
}
```

### Functionality

The firmware includes the necessary functions to handle ESP-NOW messages. It provides handlers for different command codes, such as PING, PONG, PLAY_TONE, etc. These handlers are responsible for processing and responding to ESP-NOW messages.

## Melodies

The firmware includes functionality to manage melodies, which are sequences of tones. It provides functions to create, update, and delete melodies, as well as play melodies by name.

The Melodies functionality includes the following functions:

- `void createDefaultMelodies()`: Creates default melodies.

- `void saveMelodyToFile(const String& melodyName, const melody::Melody& melody)`: Saves a melody to a `.tune` file.

- `bool loadMelodyFromFile(const String& melodyName, melody::Melody& melody)`: Loads a melody from a `.tune` file. Returns `true` if the loading is successful, `false` otherwise.

- `void listTuneFiles()`: Lists all `.tune` files on the storage.

- `void playMelody(const String& melodyName)`: Plays a melody by name.

- `bool deleteTuneFile(const String& fileName)`: Deletes a `.tune` file by filename. Returns `true` if the deletion is successful, `false` otherwise.

- `melody::Melody createMelody(const String& melodyName)`: Creates a new melody object with the given name.

- `void addToneToMelody(melody::Melody& melody, float frequency, int duration)`: Adds a tone to a melody.

## Todo


- Implement the specific logic for connecting to WiFi networks configured in the `settings.bin` file.

- Connect and subscribe to an MQTT broker by utilizing the provided functions.

- Implement the necessary code for handling the ESP-NOW protocol, including message handlers and communication functions.

- Implement a user interface (UI) to interact with the API endpoints for updating settings.

- Implement functionality to update the current time by syncing with an NTP server.

- Ensure that you have the required hardware, such as an ESP32 device, and that you properly set up the necessary dependencies and libraries in your project.

 
