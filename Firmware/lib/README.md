# AP Config

The `ap_config.h` file provides functions to manage the AP (Access Point) configurations. It allows updating, retrieving, and deleting AP configurations. These functions are used to interact with the AP configurations stored in the `settings.bin` file.

## Functions

- `bool updateAPConfigPasswordByIndex(int index, const String& password)`: Updates the password of the AP configuration at the specified index.

- `bool updateAPConfigChannelByIndex(int index, int channel)`: Updates the channel of the AP configuration at the specified index.

- `bool updateAPConfigHiddenByIndex(int index, bool hidden)`: Updates the hidden status of the AP configuration at the specified index.

- `bool updateAPConfigMaxConnectionByIndex(int index, int maxConnection)`: Updates the maximum connection count of the AP configuration at the specified index.

- `bool updateAPConfigDNSConfigPrimaryDNSByIndex(int index, const config::IPAddress& primaryDNS)`: Updates the primary DNS of the AP configuration at the specified index.

- `bool updateAPConfigDNSConfigSecondaryDNSByIndex(int index, const config::IPAddress& secondaryDNS)`: Updates the secondary DNS of the AP configuration at the specified index.

- `bool updateAPConfigDHCPConfigIPByIndex(int index, const config::IPAddress& ip)`: Updates the IP address of the AP configuration at the specified index.

- `bool updateAPConfigDHCPConfigSubnetByIndex(int index, const config::IPAddress& subnet)`: Updates the subnet mask of the AP configuration at the specified index.

- `bool updateAPConfigDHCPConfigGatewayByIndex(int index, const config::IPAddress& gateway)`: Updates the gateway of the AP configuration at the specified index.

- `String getAPConfigSSIDByIndex(int index)`: Retrieves the SSID of the AP configuration at the specified index.

- `String getAPConfigPasswordByIndex(int index)`: Retrieves the password of the AP configuration at the specified index.

- `int getAPConfigChannelByIndex(int index)`: Retrieves the channel of the AP configuration at the specified index.

- `bool getAPConfigHiddenByIndex(int index)`: Checks if the AP configuration at the specified index is hidden.

- `int getAPConfigMaxConnectionByIndex(int index)`: Retrieves the maximum connection count of the AP configuration at the specified index.

- `IPAddress getAPConfigDNSConfigPrimaryDNSByIndex(int index)`: Retrieves the primary DNS of the AP configuration at the specified index.

- `IPAddress getAPConfigDNSConfigSecondaryDNSByIndex(int index)`: Retrieves the secondary DNS of the AP configuration at the specified index.

- `IPAddress getAPConfigDHCPConfigIPByIndex(int index)`: Retrieves the IP address of the AP configuration at the specified index.

- `IPAddress getAPConfigDHCPConfigSubnetByIndex(int index)`: Retrieves the subnet mask of the AP configuration at the specified index.

- `IPAddress getAPConfigDHCPConfigGatewayByIndex(int index)`: Retrieves the gateway of the AP configuration at the specified index.

- `int countAPConfigs()`: Counts the number of AP configurations.

- `void listAPConfigs()`: Lists all the AP configurations.

- `bool readAPConfigByIndex(int index, config::APConfig& apConfig)`: Reads the AP configuration at the specified index and stores it in the provided `apConfig` object.

- `bool deleteAPConfigByIndex(int index)`: Deletes the AP configuration at the specified index.



# API Server

The `api_server.h` file provides the necessary declarations for handling API requests and starting the API server. It also includes endpoint handlers for various configurations and functionalities.

## Functions

- `void handleAPIRequest(AsyncWebServerRequest *request)`: Handles the incoming API request and routes it to the appropriate endpoint handler.

- `void startAPIServer()`: Starts the API server and begins listening for incoming requests.

- Endpoint handlers for NTPConfig:
  - `void handleNTPConfigGet()`: Handles the GET request for retrieving the NTP configuration.

- Endpoint handlers for DeviceName:
  - `void handleDeviceNameGet()`: Handles the GET request for retrieving the device name.
  - `void handleDeviceNameUpdate()`: Handles the POST request for updating the device name.

- Endpoint handlers for BootSequence:
  - `void handleBootSequenceGet()`: Handles the GET request for retrieving the boot sequence.
  - `void handleBootSequenceUpdate()`: Handles the POST request for updating the boot sequence.

- Endpoint handlers for MQTTConfig:
  - `void handleMQTTConfigGet()`: Handles the GET request for retrieving the MQTT configuration.
  - `void handleMQTTConfigUpdate()`: Handles the POST request for updating the MQTT configuration.

- Endpoint handlers for MQTTConfig - Individual fields:
  - `void handleMQTTConfigHostUpdate()`: Handles the POST request for updating the MQTT host.
  - `void handleMQTTConfigPortUpdate()`: Handles the POST request for updating the MQTT port.
  - `void handleMQTTConfigKeepAliveUpdate()`: Handles the POST request for updating the MQTT keep-alive.
  - `void handleMQTTConfigCleanSessionUpdate()`: Handles the POST request for updating the MQTT clean session.
  - `void handleMQTTConfigTimeoutUpdate()`: Handles the POST request for updating the MQTT timeout.

- Endpoint handlers for MQTTConfig - Topics:
  - `void handleMQTTConfigPublishTopicsList()`: Handles the GET request for retrieving the list of MQTT publish topics.
  - `void handleMQTTConfigSubscribeTopicsList()`: Handles the GET request for retrieving the list of MQTT subscribe topics.
  - `void handleMQTTConfigPublishTopicAdd()`: Handles the POST request for adding an MQTT publish topic.
  - `void handleMQTTConfigSubscribeTopicAdd()`: Handles the POST request for adding an MQTT subscribe topic.
  - `void handleMQTTConfigPublishTopicDelete()`: Handles the POST request for deleting an MQTT publish topic.
  - `void handleMQTTConfigSubscribeTopicDelete()`: Handles the POST request for deleting an MQTT subscribe topic.

- Endpoint handlers for SoftAPConfig:
  - `void handleSoftAPConfigGet()`: Handles the GET request for retrieving the SoftAP configuration.
  - `void handleSoftAPConfigUpdate()`: Handles the POST request for updating the SoftAP configuration.
  - `void handleSoftAPConfigSSIDGet()`: Handles the GET request for retrieving the SoftAP SSID.
  - `void handleSoftAPConfigPasswordGet()`: Handles the GET request for retrieving the SoftAP password.
  - `void handleSoftAPConfigChannelGet()`: Handles the GET request for retrieving the SoftAP channel.
  - `void handleSoftAPConfigHiddenGet()`: Handles the GET request for retrieving the SoftAP hidden status.
  - `void handleSoftAPConfigMax_ConnectionGet()`: Handles the GET request for retrieving the SoftAP maximum connection count.
  - `void handleSoftAPConfigSSIDUpdate()`: Handles the POST request for updating the SoftAP SSID.
  - `void handleSoftAPConfigPasswordUpdate()`: Handles the POST request for updating the SoftAP password.
  - `void handleSoftAPConfigChannelUpdate()`: Handles the POST request for updating the SoftAP channel.
  - `void handleSoftAPConfigHiddenUpdate()`: Handles the POST request for updating the SoftAP hidden status.
  - `void handleSoftAPConfigMax_ConnectionUpdate()`: Handles the POST request for updating the SoftAP maximum connection count.

- Endpoint handlers for AP Configurations:
  - `void handleApconfigsList()`: Handles the GET request for listing all AP configurations.
  - `void handleApconfigsGet()`: Handles the GET request for retrieving a specific AP configuration.
  - `void handleApconfigsDelete()`: Handles the DELETE request for deleting a specific AP configuration.
  - `void handleApconfigUpdate()`: Handles the POST request for updating a specific AP configuration.

- Endpoint handlers for WiFi Configurations:
  - `void handleAPConfigSSIDGet()`: Handles the GET request for retrieving the AP configuration SSID.
  - `void handleAPConfigPasswordGet()`: Handles the GET request for retrieving the AP configuration password.
  - `void handleAPConfigChannelGet()`: Handles the GET request for retrieving the AP configuration channel.
  - `void handleAPConfigHiddenGet()`: Handles the GET request for retrieving the AP configuration hidden status.
  - `void handleAPConfigMax_ConnectionGet()`: Handles the GET request for retrieving the AP configuration maximum connection count.
  - `void handleAPConfigSSIDUpdate()`: Handles the POST request for updating the AP configuration SSID.
  - `void handleAPConfigPasswordUpdate()`: Handles the POST request for updating the AP configuration password.
  - `void handleAPConfigChannelUpdate()`: Handles

the POST request for updating the AP configuration channel.
  - `void handleAPConfigHiddenUpdate()`: Handles the POST request for updating the AP configuration hidden status.
  - `void handleAPConfigMax_ConnectionUpdate()`: Handles the POST request for updating the AP configuration maximum connection count.
  - `void handleAPConfigGetdhcp_config_ip()`: Handles the GET request for retrieving the AP configuration DHCP IP address.
  - `void handleAPConfigGetdhcp_config_gateway()`: Handles the GET request for retrieving the AP configuration DHCP gateway.
  - `void handleAPConfigGetdhcp_config_subnet()`: Handles the GET request for retrieving the AP configuration DHCP subnet mask.
  - `void handleAPConfigUpdatedhcp_config_ip()`: Handles the POST request for updating the AP configuration DHCP IP address.
  - `void handleAPConfigUpdatedhcp_config_gateway()`: Handles the POST request for updating the AP configuration DHCP gateway.
  - `void handleAPConfigUpdatedhcp_config_subnet()`: Handles the POST request for updating the AP configuration DHCP subnet mask.
  - `void handleDNSConfigGet()`: Handles the GET request for retrieving the DNS configuration.
  - `void handleDNSConfigUpdate()`: Handles the POST request for updating the DNS configuration.
  - `void handleDNSConfigDelete()`: Handles the DELETE request for deleting the DNS configuration.
  - `void handleDNSConfigGetPrimaryDNS()`: Handles the GET request for retrieving the primary DNS configuration.
  - `void handleDNSConfigUpdatePrimaryDNS()`: Handles the POST request for updating the primary DNS configuration.
  - `void handleDNSConfigGetSecondaryDNS()`: Handles the GET request for retrieving the secondary DNS configuration.
  - `void handleDNSConfigUpdateSecondaryDNS()`: Handles the POST request for updating the secondary DNS configuration.
  - `void handleDNSConfigPrimary_dnsGet()`: Handles the GET request for retrieving the DNS primary DNS.
  - `void handleDNSConfigSecondary_dnsGet()`: Handles the GET request for retrieving the DNS secondary DNS.
  - `void handleDNSConfigPrimary_dnsUpdate()`: Handles the POST request for updating the DNS primary DNS.
  - `void handleDNSConfigSecondary_dnsUpdate()`: Handles the POST request for updating the DNS secondary DNS.
  - `void handleDHCPConfigGet()`: Handles the GET request for retrieving the DHCP configuration.
  - `void handleDHCPConfigUpdate()`: Handles the POST request for updating the DHCP configuration.
  - `void handleDHCPConfigDelete()`: Handles the DELETE request for deleting the DHCP configuration.

# Device

The `device.h` file provides the necessary declarations for managing the device name in the settings. It includes functions to update and read the device name.

## Functions

- `bool updateDeviceName(const String& deviceName)`: Updates the device name in the settings with the specified `deviceName` parameter. Returns `true` if the update is successful; otherwise, returns `false`.

- `String readDeviceName()`: Reads and returns the device name from the settings.

# ESP Now Listener

The `esp_now_listener.h` file provides the necessary declarations for setting up an ESP-NOW listener. It includes functions to handle different command codes, a callback for when data is received, and a function to initialize ESP-NOW.

## Functions

- `void handle_ping()`: Handles the "ping" command code.

- `void handle_pong()`: Handles the "pong" command code.

- `void handle_play_tone()`: Handles the "play_tone" command code.

- `void handle_play_tone_ok()`: Handles the "play_tone_ok" command code.

- `void handle_play_tx()`: Handles the "play_tx" command code.

- `void handle_play_tx_ok()`: Handles the "play_tx_ok" command code.

- `void handle_sync_ap()`: Handles the "sync_ap" command code.

- `void handle_sync_ap_ok()`: Handles the "sync_ap_ok" command code.

- `void handle_reboot()`: Handles the "reboot" command code.

- `void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len)`: Callback function that is triggered when data is received. It takes in the MAC address of the sender, the incoming data, and the length of the data as parameters.

- `void initESP_Now()`: Function to initialize ESP-NOW.

# Melodies

The `melodies.h` file provides functions for working with melodies in your Arduino project. It includes functions to create, save, load, list, play, and delete melodies.

## Functions

- `void createDefaultMelodies()`: Creates the default melodies.

- `void saveMelodyToFile(const String& melodyName, const melody::Melody& melody)`: Saves a melody to a .tune file with the specified name.

- `bool loadMelodyFromFile(const String& melodyName, melody::Melody& melody)`: Loads a melody from a .tune file with the specified name.

- `void listTuneFiles()`: Lists all .tune files on the LittleFS partition.

- `void playMelody(const String& melodyName)`: Plays the melody with the specified name.

- `bool deleteTuneFile(const String& fileName)`: Deletes the .tune file with the specified filename.

- `melody::Melody createMelody(const String& melodyName)`: Creates a melody object with the given name.

- `void addToneToMelody(melody::Melody& melody, float freq, int duration)`: Adds a tone to the specified melody.

# MQTT Config

The `mqtt_config.h` file provides functions for working with MQTT configurations in your Arduino project. It includes functions to delete MQTT configurations, read MQTT configurations, update MQTT configuration properties such as host, port, keep-alive, clean session, and timeout, count the number of MQTT publish and subscribe topics, update MQTT publish and subscribe topics by index, delete MQTT publish and subscribe topics by index, add new MQTT publish and subscribe topics, and retrieve MQTT publish and subscribe topics by index.

## Functions

- `bool deleteMQTTConfig()`: Deletes the MQTT configuration.

- `bool readMQTTConfig(config::MQTTConfig& MQTTConfig)`: Reads the MQTT configuration and stores it in the provided `config::MQTTConfig` object.

- `bool updateMQTTConfighost(String& host)`: Updates the MQTT configuration host property with the specified value.

- `bool updateMQTTConfigport(int& port)`: Updates the MQTT configuration port property with the specified value.

- `bool updateMQTTConfigkeep_alive(bool& keep_alive)`: Updates the MQTT configuration keep-alive property with the specified value.

- `bool updateMQTTConfigclean_session(bool& clean_session)`: Updates the MQTT configuration clean session property with the specified value.

- `bool updateMQTTConfig(int& timeout)`: Updates the MQTT configuration timeout property with the specified value.

- `int countMQTTConfigpublish_topics()`: Returns the number of MQTT publish topics in the configuration.

- `int countMQTTConfigsubscribe_topics()`: Returns the number of MQTT subscribe topics in the configuration.

- `bool updateMQTTConfigpublish_topicByIndex(int index, String& topic)`: Updates the MQTT publish topic at the specified index with the specified value.

- `bool updateMQTTConfigsubscribe_topicByIndex(int index, String& topic)`: Updates the MQTT subscribe topic at the specified index with the specified value.

- `bool deleteMQTTConfigpublish_topicByIndex(int index)`: Deletes the MQTT publish topic at the specified index.

- `bool deleteMQTTConfigsubscribe_topicByIndex(int index)`: Deletes the MQTT subscribe topic at the specified index.

- `bool addMQTTConfigpublish_topic(String& topic)`: Adds a new MQTT publish topic with the specified value.

- `bool addMQTTConfigsubscribe_topic(String& topic)`: Adds a new MQTT subscribe topic with the specified value.

- `String getMQTTConfigpublish_topicByIndex(int index)`: Retrieves the MQTT publish topic at the specified index.

- `String getMQTTConfigsubscribe_topicByIndex(int index)`: Retrieves the MQTT subscribe topic at the specified index.

## Usage

To use the MQTT Config functionality, include the `mqtt_config.h` file in your project.

```cpp
#include "mqtt_config.h"

void setup() {
  // Initialize your setup

  // Set up other necessary configurations and handlers

  // Other setup operations
}

void loop() {
  // Handle any necessary operations in the loop
}
```

Make sure to include the necessary dependencies, such as `Arduino` and `config.pb.h`.

Use the provided functions to perform various operations on the MQTT configuration. For example, you can use the `deleteMQTTConfig` function to delete the MQTT configuration, the `readMQTTConfig` function to read and retrieve the MQTT configuration, and the `updateMQTTConfighost`, `updateMQTTConfigport`, `updateMQTTConfigkeep_alive`, `updateMQTTConfigclean_session`, and `updateMQTTConfig` functions to update specific properties of the MQTT configuration.

The `countMQTTConfigpublish_topics` function and `countMQTTConfigsubscribe_topics` function allow you to count the number of MQTT publish and subscribe topics in the configuration, respectively.

To update MQTT publish and subscribe topics, you can use the `updateMQTTConfigpublish_topicByIndex`, `updateMQTTConfigsubscribe_topicByIndex`, `deleteMQTTConfigpublish_topicByIndex`, and `deleteMQTTConfigsubscribe_topicByIndex` functions. These functions allow you to update or delete specific topics by specifying the index.

The `addMQTTConfigpublish_topic` and `addMQTTConfigsubscribe_topic` functions enable you to add new MQTT publish and subscribe topics to the configuration, respectively.

To retrieve MQTT publish and subscribe topics by index, you can use the `getMQTTConfigpublish_topicByIndex` and `getMQTTConfigsubscribe_topicByIndex` functions.

Ensure that you have properly set up the necessary dependencies and libraries related to working with MQTT configurations, such as the `config::MQTTConfig` object.


# Settings Flags

The `settings_flags.h` file provides functions to work with different flags related to settings in your Arduino project. It includes functions to check if custom mDNS, custom DHCP, custom NTP, and MQTT are enabled or disabled. Additionally, it provides functions to enable or disable these flags.

## Functions

- `bool is_custom_mdns_enabled()`: Checks if custom mDNS is enabled.

- `bool is_custom_dhcp_enabled()`: Checks if custom DHCP is enabled.

- `bool is_custom_ntp_enabled()`: Checks if custom NTP is enabled.

- `bool is_mqtt_enabled()`: Checks if MQTT is enabled.

- `bool custom_mdns_enabled(bool enable)`: Enables or disables custom mDNS. Returns `true` if the operation is successful, `false` otherwise.

- `bool custom_dhcp_enabled(bool enable)`: Enables or disables custom DHCP. Returns `true` if the operation is successful, `false` otherwise.

- `bool custom_ntp_enabled(bool enable)`: Enables or disables custom NTP. Returns `true` if the operation is successful, `false` otherwise.

- `bool mqtt_enabled(bool enable)`: Enables or disables MQTT. Returns `true` if the operation is successful, `false` otherwise.

## Usage

To use the Settings Flags functionality, include the `settings_flags.h` file in your project.

```cpp
#include "settings_flags.h"

void setup() {
  // Initialize your setup

  // Set up other necessary configurations and handlers

  // Other setup operations
}

void loop() {
  // Handle any necessary operations in the loop
}
```

Use the provided functions to interact with the different flags related to settings. For example, you can use the `is_custom_mdns_enabled` function to check if custom mDNS is enabled, the `is_custom_dhcp_enabled` function to check if custom DHCP is enabled, the `is_custom_ntp_enabled` function to check if custom NTP is enabled, and the `is_mqtt_enabled` function to check if MQTT is enabled.

To enable or disable a specific flag, use the corresponding `custom_*_enabled` function, where `*` represents the specific flag. Pass `true` to enable the flag or `false` to disable it. The function will return `true` if the operation is successful, indicating that the flag has been updated, or `false` if there was an error during the operation.

Ensure that you have properly set up the necessary dependencies and libraries related to the settings and flags in your project.

# Settings

The `settings.h` file provides functions to work with device settings in your Arduino project. It includes functions to load and save the device settings from/to a `settings.bin` file, delete the settings file, and create default settings if the file doesn't exist.

## Functions

- `bool loadSettings(config::Settings& settings)`: Loads the device settings from the `settings.bin` file and populates the provided `config::Settings` object. Returns `true` if the settings are successfully loaded, `false` otherwise.

- `bool saveSettings(const config::Settings& settings)`: Saves the provided device settings to the `settings.bin` file. Returns `true` if the settings are successfully saved, `false` otherwise.

- `void deleteSettings()`: Deletes the `settings.bin` file.

- `void createDefaultSettings()`: Creates default settings if the `settings.bin` file doesn't exist. This function is called to initialize the settings file with default values.

