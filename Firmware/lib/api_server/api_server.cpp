#include "api_server.h"
#include "json_helpers.h" // Include the file that contains JSON helper functions
#include "settings_helpers.h" // Include the file that contains helper functions for settings

void handleAPIRequest(AsyncWebServerRequest *request) {
  // Handle API request here
  request->send(200, "text/plain", "Hello from API server");
}

void startAPIServer() {
  // Handle all API requests with the same handler
  server.on("/api", HTTP_GET, handleAPIRequest);
    // Endpoint handlers

  server.on("/api/v1/settings/device_name/", HTTP_GET, handleDeviceNameGet);
  server.on("/api/v1/settings/device_name/", HTTP_POST, handleDeviceNameUpdate);
  
  server.on("/api/v1/settings/boot_sequence/", HTTP_GET, handleBootSequenceGet);
  server.on("/api/v1/settings/boot_sequence/", HTTP_POST, handleBootSequenceUpdate);
  

  // Entire MQTT configuration
  server.on("/api/v1/settings/mqtt_config/", HTTP_POST, handleMQTTConfigUpdate);
  server.on("/api/v1/settings/mqtt_config/", HTTP_GET, handleMQTTConfigGet);
  
  // MQTT host
  server.on("/api/v1/settings/mqtt_config/host/", HTTP_POST, handleMQTTConfigHostUpdate);
  server.on("/api/v1/settings/mqtt_config/host/", HTTP_GET, handleMQTTConfigHostGet);

  // MQTT port
  server.on("/api/v1/settings/mqtt_config/port/", HTTP_POST, handleMQTTConfigPortUpdate);
  server.on("/api/v1/settings/mqtt_config/port/", HTTP_GET, handleMQTTConfigPortGet);


  // MQTT keep_alive
  server.on("/api/v1/settings/mqtt_config/keep_alive/", HTTP_POST, handleMQTTConfigKeepAliveUpdate);
  server.on("/api/v1/settings/mqtt_config/keep_alive/", HTTP_GET, handleMQTTConfigKeepAliveGet);

  // MQTT clean_session
  server.on("/api/v1/settings/mqtt_config/clean_session/", HTTP_POST, handleMQTTConfigCleanSessionUpdate);
  server.on("/api/v1/settings/mqtt_config/clean_session/", HTTP_GET, handleMQTTConfigCleanSessionGet);

  // MQTT timeout
  server.on("/api/v1/settings/mqtt_config/timeout/", HTTP_POST, handleMQTTConfigTimeoutUpdate);
  server.on("/api/v1/settings/mqtt_config/timeout/", HTTP_GET, handleMQTTConfigTimeoutGet);

  // Get MQTT topics list
  server.on("/api/v1/settings/mqtt_config/publish_topics/", HTTP_GET, handleMQTTConfigPublishTopicsList);
  server.on("/api/v1/settings/mqtt_config/subscribe_topics/", HTTP_GET, handleMQTTConfigSubscribeTopicsList);

  // Add MQTT topic
  server.on("/api/v1/settings/mqtt_config/publish_topic/add", HTTP_POST, handleMQTTConfigPublishTopicAdd);
  server.on("/api/v1/settings/mqtt_config/subscribe_topic/add", HTTP_POST, handleMQTTConfigSubscribeTopicAdd);

  // Delete MQTT topics
  server.on("/api/v1/settings/mqtt_config/publish_topic/<index>", HTTP_DELETE, handleMQTTConfigPublishTopicDelete);
  server.on("/api/v1/settings/mqtt_config/subscribe_topic/<index>", HTTP_DELETE, handleMQTTConfigSubscribeTopicDelete);

   // Get MQTT topics by index
  server.on("/api/v1/settings/mqtt_config/publish_topic/<index>", HTTP_DELETE, handleMQTTConfigPublishTopicGetByIndex);
  server.on("/api/v1/settings/mqtt_config/subscribe_topic/<index>", HTTP_DELETE, handleMQTTConfigSubscribeTopicGetByIndex);
  
  // soft_ap configs
  server.on("/api/v1/settings/soft_ap_config/", HTTP_GET, handleSoftAPConfigGet);
  server.on("/api/v1/settings/soft_ap_config/", HTTP_POST, handleSoftAPConfigUpdate); 
  server.on("/api/v1/settings/soft_ap_config/SSID" , HTTP_GET, handleSoftAPConfigSSIDGet);
  server.on("/api/v1/settings/soft_ap_config/password", HTTP_GET, handleSoftAPConfigPasswordGet);
  server.on("/api/v1/settings/soft_ap_config/channel", HTTP_GET, handleSoftAPConfigChannelGet);
  server.on("/api/v1/settings/soft_ap_config/hidden", HTTP_GET, handleSoftAPConfigHiddenGet);
  server.on("/api/v1/settings/soft_ap_config/max_connection", HTTP_GET, handleSoftAPConfigMax_ConnectionGet);
 
  server.on("/api/v1/settings/soft_ap_config/SSID" , HTTP_POST, handleSoftAPConfigSSIDUpdate);
  server.on("/api/v1/settings/soft_ap_config/password", HTTP_POST, handleSoftAPConfigPasswordUpdate);
  server.on("/api/v1/settings/soft_ap_config/channel", HTTP_POST, handleSoftAPConfigChannelUpdate);
  server.on("/api/v1/settings/soft_ap_config/hidden", HTTP_POST, handleSoftAPConfigHiddenUpdate);
  server.on("/api/v1/settings/soft_ap_config/max_connection", HTTP_POST, handleSoftAPConfigMax_ConnectionUpdate);  
   
  // wifi_config/ap_configs 
  server.on("/api/v1/settings/wifi_config/ap_configs/list", HTTP_GET, handleAPConfigsList);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>", HTTP_GET, handleAPConfigsGet);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>", HTTP_DELETE, handleAPConfigsDelete);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>", HTTP_POST, handleAPConfigUpdate);
  
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/SSID" , HTTP_GET, handleAPConfigSSIDGet);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/password", HTTP_GET, handleAPConfigPasswordGet);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/channel", HTTP_GET, handleAPConfigChannelGet);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/hidden", HTTP_GET, handleAPConfigHiddenGet);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/max_connection", HTTP_GET, handleAPConfigMax_ConnectionGet);
  
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/SSID" , HTTP_POST, handleAPConfigSSIDUpdate);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/password", HTTP_POST, handleAPConfigPasswordUpdate);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/channel", HTTP_POST, handleAPConfigChannelUpdate);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/hidden", HTTP_POST, handleAPConfigHiddenUpdate);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/max_connection", HTTP_POST, handleAPConfigMax_ConnectionUpdate);
   
  //DHCP Config 
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/dhcp_config", HTTP_GET, handleDHCPConfigGet);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/dhcp_config", HTTP_POST, handleDHCPConfigUpdate);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/dhcp_config", HTTP_DELETE, handleDHCPConfigDelete);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/dhcp_config/ip", HTTP_GET, handleAPConfigGetdhcp_config_ip);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/dhcp_config/gateway"HTTP_GET, handleAPConfigGetdhcp_config_gateway);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/dhcp_config/subnet" HTTP_GET, handleAPConfigGetdhcp_config_subnet);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/dhcp_config/ip", HTTP_POST, handleAPConfigUpdatedhcp_config_ip);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/dhcp_config/gateway"HTTP_POST, handleAPConfigUpdatedhcp_config_gateway);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/dhcp_config/subnet" HTTP_POST, handleAPConfigUpdatedhcp_config_subnet);
  
  
  // DNS Config
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/dns_config", HTTP_GET, handleDNSConfigGet);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/dns_config", HTTP_POST, handleDNSConfigUpdate); 
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/dns_config", HTTP_DELETE, handleDNSConfigDelete); 
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/dns_config/primary_dns", HTTP_GET, handleDNSConfigPrimary_dnsGet);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/dns_config/secondary_dns", HTTP_GET, handleDNSConfigSecondary_dnsGet);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/dns_config/primary_dns", HTTP_POST, handleDNSConfigPrimary_dnsUpdate);
  server.on("/api/v1/settings/wifi_config/ap_configs/<index>/dns_config/secondary_dns", HTTP_POST, handleDNSConfigSecondary_dnsUpdate);




  

  //ntp_configs 
  server.on("/api/v1/settings/ntp_config/", HTTP_GET, handleNTPConfigGet);
  server.on("/api/v1/settings/ntp_config/", HTTP_GET, handleNTPConfigGet);

  
  server.begin();
}

// Handler for /api/v1/settings/ntp_config/get
void handleNTPConfigGet() {
  // Retrieve NTP configuration using the provided function
  config::NTPConfig ntpConfig;
  bool success = readNTPConfig(ntpConfig);

  // Create a JSON response with the NTP configuration
  DynamicJsonDocument jsonDoc(128);
  jsonDoc["success"] = success;
  jsonDoc["ntpConfig"]["pool_server_name"] = ntpConfig.pool_server_name();
  jsonDoc["ntpConfig"]["port"] = ntpConfig.port();
  String jsonResponse;
  serializeJson(jsonDoc, jsonResponse);

  // Send the JSON response
  server.send(200, "application/json", jsonResponse);
}

// Handler for /api/v1/settings/device_name/get
void handleDeviceNameGet() {
  // Retrieve device name using the provided function
  String deviceName = readDeviceName();

  // Create a JSON response with the device name
  DynamicJsonDocument jsonDoc(128);
  jsonDoc["deviceName"] = deviceName;
  String jsonResponse;
  serializeJson(jsonDoc, jsonResponse);

  // Send the JSON response
  server.send(200, "application/json", jsonResponse);
}

// Handler for /api/v1/settings/device_name/update
void handleDeviceNameUpdate() {
  // Retrieve the new device name from the JSON body
  String deviceName = getRequestBodyValue("deviceName");

  // Update the device name using the provided function
  bool success = updateDeviceName(deviceName);

  // Create a JSON response with the update status
  DynamicJsonDocument jsonDoc(128);
  jsonDoc["success"] = success;
  String jsonResponse;
  serializeJson(jsonDoc, jsonResponse);

  // Send the JSON response
  server.send(200, "application/json", jsonResponse);
}

// Handler for /api/v1/settings/BootSequence/get
void handleBootSequenceGet() {
  // Retrieve the boot sequence using the provided function
  BootSequence bootSequence = readBootSequence();

  // Create a JSON response with the boot sequence
  DynamicJsonDocument jsonDoc(128);
  jsonDoc["bootSequence"] = static_cast<int>(bootSequence);
  String jsonResponse;
  serializeJson(jsonDoc, jsonResponse);

  // Send the JSON response
  server.send(200, "application/json", jsonResponse);
}

// Handler for /api/v1/settings/BootSequence/update
void handleBootSequenceUpdate() {
  // Retrieve the new boot sequence from the JSON body
  int bootSequenceValue = getRequestBodyValue("bootSequence");
  BootSequence bootSequence = static_cast<BootSequence>(bootSequenceValue);

  // Update the boot sequence using the provided function
  bool success = updateBootSequence(bootSequence);

  // Create a JSON response with the update status
  DynamicJsonDocument jsonDoc(128);
  jsonDoc["success"] = success;
  String jsonResponse;
  serializeJson(jsonDoc, jsonResponse);

  // Send the JSON response
  server.send(200, "application/json", jsonResponse);
}

// Handler for /api/v1/settings/MQTTConfig/get
void handleMQTTConfigGet() {
  // Retrieve MQTT configuration using the provided function
  config::MQTTConfig mqttConfig;
  bool success = readMQTTConfig(mqttConfig);

  // Create a JSON response with the MQTT configuration
  DynamicJsonDocument jsonDoc(128);
  jsonDoc["success"] = success;
  jsonDoc["mqttConfig"]["host"] = mqttConfig.host();
  jsonDoc["mqttConfig"]["port"] = mqttConfig.port();
  // Add other MQTT configuration fields

  String jsonResponse;
  serializeJson(jsonDoc, jsonResponse);

  // Send the JSON response
  server.send(200, "application/json", jsonResponse);
}

// Handler for /api/v1/settings/MQTTConfig/update
void handleMQTTConfigUpdate() {
  // Retrieve the new MQTT configuration from the JSON body
  String host = getRequestBodyValue("host");
  int port = getRequestBodyValue("port");
  // Retrieve other MQTT configuration fields

  // Update the MQTT configuration using the provided function
  bool success = updateMQTTConfig(host, port);
  // Update other MQTT configuration fields

  // Create a JSON response with the update status
  DynamicJsonDocument jsonDoc(128);
  jsonDoc["success"] = success;
  String jsonResponse;
  serializeJson(jsonDoc, jsonResponse);

  // Send the JSON response
  server.send(200, "application/json", jsonResponse);
}

// Handler for /api/v1/settings/SoftAPConfig/get
void handleSoftAPConfigGet() {
  // Retrieve SoftAP configuration using the provided function
  config::SoftAPConfig softAPConfig;
  bool success = readSoftAPConfig(softAPConfig);

  // Create a JSON response with the SoftAP configuration
  DynamicJsonDocument jsonDoc(128);
  jsonDoc["success"] = success;
  jsonDoc["softAPConfig"]["SSID"] = softAPConfig.SSID();
  jsonDoc["softAPConfig"]["password"] = softAPConfig.password();
  // Add other SoftAP configuration fields

  String jsonResponse;
  serializeJson(jsonDoc, jsonResponse);

  // Send the JSON response
  server.send(200, "application/json", jsonResponse);
}

// Handler for /api/v1/settings/SoftAPConfig/update
void handleSoftAPConfigUpdate() {
  // Retrieve the new SoftAP configuration from the JSON body
  String ssid = getRequestBodyValue("SSID");
  String password = getRequestBodyValue("password");
  // Retrieve other SoftAP configuration fields

  // Update the SoftAP configuration using the provided function
  bool success = updateSoftAPConfig(ssid, password);
  // Update other SoftAP configuration fields

  // Create a JSON response with the update status
  DynamicJsonDocument jsonDoc(128);
  jsonDoc["success"] = success;
  String jsonResponse;
  serializeJson(jsonDoc, jsonResponse);

  // Send the JSON response
  server.send(200, "application/json", jsonResponse);
}

// Handler for /api/v1/settings/wificonfig/apconfigs/list
 
 
 void handleAPConfigsList() {
  DynamicJsonDocument json(1024);

  // Create an array to hold the APConfig objects
  JsonArray apConfigsArray = json.createNestedArray("apConfigs");

  // Get the count of APConfigs
  int count = countAPConfigs();

  // Iterate through each APConfig and add it to the array
  for (int i = 0; i < count; i++) {
    JsonObject apConfigObj = apConfigsArray.createNestedObject();
    apConfigObj["index"] = i;
    apConfigObj["ssid"] = getAPConfigSSIDByIndex(i);
  }

  String jsonResponse;
  serializeJson(json, jsonResponse);

  server.send(200, "application/json", jsonResponse);
}

void handleDNSConfigGetPrimaryDNS() {
  int index = getAPConfigIndexFromURL(); // Retrieve the index from the URL

  // Read the primary DNS configuration for the specified APConfig
  IPAddress primaryDNS = getAPConfigDNSConfigPrimaryDNSByIndex(index);

  // Create a JSON response
  DynamicJsonDocument json(128);
  json["primary_dns"] = ipToString(primaryDNS); // Convert the IPAddress to string

  String jsonResponse;
  serializeJson(json, jsonResponse);

  server.send(200, "application/json", jsonResponse);
}

void handleDNSConfigUpdatePrimaryDNS() {
  int index = getAPConfigIndexFromURL(); // Retrieve the index from the URL

  // Retrieve the new primary DNS configuration from the request body
  String requestBody = server.arg("plain");
  DynamicJsonDocument json(128);
  deserializeJson(json, requestBody);

  // Extract the primary_dns value from the JSON
  String primaryDNSString = json["primary_dns"];

  // Convert the primary_dns value to an IPAddress object
  IPAddress primaryDNS = stringToIP(primaryDNSString);

  // Update the primary DNS configuration for the specified APConfig
  bool success = updateAPConfigDNSConfigPrimaryDNSByIndex(index, primaryDNS);

  if (success) {
    server.send(200, "text/plain", "Primary DNS configuration updated successfully");
  } else {
    server.send(500, "text/plain", "Failed to update primary DNS configuration");
  }
}

void handleDNSConfigGetSecondaryDNS() {
  int index = getAPConfigIndexFromURL(); // Retrieve the index from the URL

  // Read the secondary DNS configuration for the specified APConfig
  IPAddress secondaryDNS = getAPConfigDNSConfigSecondaryDNSByIndex(index);

  // Create a JSON response
  DynamicJsonDocument json(128);
  json["secondary_dns"] = ipToString(secondaryDNS); // Convert the IPAddress to string

  String jsonResponse;
  serializeJson(json, jsonResponse);

  server.send(200, "application/json", jsonResponse);
}

void handleDNSConfigUpdateSecondaryDNS() {
  int index = getAPConfigIndexFromURL(); // Retrieve the index from the URL

  // Retrieve the new secondary DNS configuration from the request body
  String requestBody = server.arg("plain");
  DynamicJsonDocument json(128);
  deserializeJson(json, requestBody);

  // Extract the secondary_dns value from the JSON
  String secondaryDNSString = json["secondary_dns"];

  // Convert the secondary_dns value to an IPAddress object
  IPAddress secondaryDNS = stringToIP(secondaryDNSString);

  // Update the secondary DNS configuration for the specified APConfig
  bool success = updateAPConfigDNSConfigSecondaryDNSByIndex(index, secondaryDNS);

  if (success) {
    server.send(200, "text/plain", "Secondary DNS configuration updated successfully");
  } else {
    server.send(500, "text/plain", "Failed to update secondary DNS configuration");
  }
}



void handleMQTTConfigGet() {
  config::MQTTConfig mqttConfig;
  if (readMQTTConfig(mqttConfig)) {
    // Convert MQTT config to JSON
    DynamicJsonDocument jsonBuffer(256);
    jsonBuffer["host"] = mqttConfig.host();
    jsonBuffer["port"] = mqttConfig.port();
    jsonBuffer["keep_alive"] = mqttConfig.keep_alive();
    jsonBuffer["clean_session"] = mqttConfig.clean_session();
    jsonBuffer["timeout"] = mqttConfig.timeout();

    String jsonStr;
    serializeJson(jsonBuffer, jsonStr);

    server.send(200, "application/json", jsonStr);
  } else {
    server.send(500, "text/plain", "Failed to read MQTT config");
  }
}

void handleMQTTConfigUpdate() {
  if (server.hasArg("plain")) {
    String payload = server.arg("plain");
    DynamicJsonDocument jsonBuffer(256);
    DeserializationError error = deserializeJson(jsonBuffer, payload);

    if (error) {
      server.send(400, "text/plain", "Invalid JSON payload");
      return;
    }

    config::MQTTConfig mqttConfig;
    if (!readMQTTConfig(mqttConfig)) {
      server.send(500, "text/plain", "Failed to read MQTT config");
      return;
    }

    if (jsonBuffer.containsKey("host")) {
      String host = jsonBuffer["host"].as<String>();
      if (!updateMQTTConfighost(host)) {
        server.send(500, "text/plain", "Failed to update MQTT host");
        return;
      }
    }

    if (jsonBuffer.containsKey("port")) {
      int port = jsonBuffer["port"].as<int>();
      if (!updateMQTTConfigport(port)) {
        server.send(500, "text/plain", "Failed to update MQTT port");
        return;
      }
    }

    if (jsonBuffer.containsKey("keep_alive")) {
      bool keepAlive = jsonBuffer["keep_alive"].as<bool>();
      if (!updateMQTTConfigkeep_alive(keepAlive)) {
        server.send(500, "text/plain", "Failed to update MQTT keep alive");
        return;
      }
    }

    if (jsonBuffer.containsKey("clean_session")) {
      bool cleanSession = jsonBuffer["clean_session"].as<bool>();
      if (!updateMQTTConfigclean_session(cleanSession)) {
        server.send(500, "text/plain", "Failed to update MQTT clean session");
        return;
      }
    }

    if (jsonBuffer.containsKey("timeout")) {
      int timeout = jsonBuffer["timeout"].as<int>();
      if (!updateMQTTConfig(timeout)) {
        server.send(500, "text/plain", "Failed to update MQTT timeout");
        return;
      }
    }

    server.send(200, "text/plain", "MQTT config updated successfully");
  } else {
    server.send(400, "text/plain", "Missing payload");
  }
}

void handleMQTTConfigPublishTopicsList() {
  int count = countMQTTConfigpublish_topics();
  DynamicJsonDocument jsonBuffer(count * 50);
  JsonArray topicsArray = jsonBuffer.to<JsonArray>();

  for (int i = 0; i < count; i++) {
    String topic = getMQTTConfigpublish_topicByIndex(i);
    topicsArray.add(topic);
  }

  String jsonStr;
  serializeJson(jsonBuffer, jsonStr);

  server.send(200, "application/json", jsonStr);
}

void handleMQTTConfigSubscribeTopicsList() {
  int count = countMQTTConfigsubscribe_topics();
  DynamicJsonDocument jsonBuffer(count * 50);
  JsonArray topicsArray = jsonBuffer.to<JsonArray>();

  for (int i = 0; i < count; i++) {
    String topic = getMQTTConfigsubscribe_topicByIndex(i);
    topicsArray.add(topic);
  }

  String jsonStr;
  serializeJson(jsonBuffer, jsonStr);

  server.send(200, "application/json", jsonStr);
}

void handleMQTTConfigPublishTopicAdd() {
  if (server.hasArg("plain")) {
    String topic = server.arg("plain");

    if (addMQTTConfigpublish_topic(topic)) {
      server.send(200, "text/plain", "MQTT publish topic added successfully");
    } else {
      server.send(500, "text/plain", "Failed to add MQTT publish topic");
    }
  } else {
    server.send(400, "text/plain", "Missing topic");
  }
}

void handleMQTTConfigSubscribeTopicAdd() {
  if (server.hasArg("plain")) {
    String topic = server.arg("plain");

    if (addMQTTConfigsubscribe_topic(topic)) {
      server.send(200, "text/plain", "MQTT subscribe topic added successfully");
    } else {
      server.send(500, "text/plain", "Failed to add MQTT subscribe topic");
    }
  } else {
    server.send(400, "text/plain", "Missing topic");
  }
}

void handleMQTTConfigPublishTopicDelete() {
  if (server.hasArg("index")) {
    int index = server.arg("index").toInt();

    if (deleteMQTTConfigpublish_topicByIndex(index)) {
      server.send(200, "text/plain", "MQTT publish topic deleted successfully");
    } else {
      server.send(500, "text/plain", "Failed to delete MQTT publish topic");
    }
  } else {
    server.send(400, "text/plain", "Missing index");
  }
}

void handleMQTTConfigSubscribeTopicDelete() {
  if (server.hasArg("index")) {
    int index = server.arg("index").toInt();

    if (deleteMQTTConfigsubscribe_topicByIndex(index)) {
      server.send(200, "text/plain", "MQTT subscribe topic deleted successfully");
    } else {
      server.send(500, "text/plain", "Failed to delete MQTT subscribe topic");
    }
  } else {
    server.send(400, "text/plain", "Missing index");
  }
}

void handleAPConfigSSIDGet() {
  // Get the index from the URL parameter
  int index = server.pathArg("index").toInt();

  // Call the corresponding function to get the APConfig SSID
  String ssid = getAPConfigSSIDByIndex(index);

  // Create a JSON response with the SSID value
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["ssid"] = ssid;

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleAPConfigPasswordGet() {
  // Get the index from the URL parameter
  int index = server.pathArg("index").toInt();

  // Call the corresponding function to get the APConfig password
  String password = getAPConfigPasswordByIndex(index);

  // Create a JSON response with the password value
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["password"] = password;

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleAPConfigChannelGet() {
  // Get the index from the URL parameter
  int index = server.pathArg("index").toInt();

  // Call the corresponding function to get the APConfig channel
  int channel = getAPConfigChannelByIndex(index);

  // Create a JSON response with the channel value
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["channel"] = channel;

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleAPConfigHiddenGet() {
  // Get the index from the URL parameter
  int index = server.pathArg("index").toInt();

  // Call the corresponding function to get the APConfig hidden status
  bool hidden = getAPConfigHiddenByIndex(index);

  // Create a JSON response with the hidden status value
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["hidden"] = hidden;

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleAPConfigMax_ConnectionGet() {
  // Get the index from the URL parameter
  int index = server.pathArg("index").toInt();

  // Call the corresponding function to get the APConfig max_connection value
  int maxConnection = getAPConfigMaxConnectionByIndex(index);

  // Create a JSON response with the max_connection value
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["max_connection"] = maxConnection;

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleAPConfigGetdhcp_config_ip() {
  // Get the index from the URL parameter
  int index = server.pathArg("index").toInt();

  // Call the corresponding function to get the APConfig DHCPConfig IP
  IPAddress ip = getAPConfigDHCPConfigIPByIndex(index);

  // Create a JSON response with the IP value
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["ip"] = ip.toString();

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleAPConfigGetdhcp_config_gateway() {
  // Get the index from the URL parameter
  int index = server.pathArg("index").toInt();

  // Call the corresponding function to get the APConfig DHCPConfig gateway
  IPAddress gateway = getAPConfigDHCPConfigGatewayByIndex(index);

  // Create a JSON response with the gateway value
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["gateway"] = gateway.toString();

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleAPConfigGetdhcp_config_subnet() {
  // Get the index from the URL parameter
  int index = server.pathArg("index").toInt();

  // Call the corresponding function to get the APConfig DHCPConfig subnet
  IPAddress subnet = getAPConfigDHCPConfigSubnetByIndex(index);

  // Create a JSON response with the subnet value
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["subnet"] = subnet.toString();

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleAPConfigUpdatedhcp_config_ip() {
  // Get the index from the URL parameter
  int index = server.pathArg("index").toInt();

  // Get the new IP value from the request body
  String ipString = server.arg("ip");
  IPAddress ip;
  ip.fromString(ipString);

  // Call the corresponding function to update the APConfig DHCPConfig IP
  bool success = updateAPConfigDHCPConfigIPByIndex(index, ip);

  // Create a JSON response with the success status
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["success"] = success;

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleAPConfigUpdatedhcp_config_gateway() {
  // Get the index from the URL parameter
  int index = server.pathArg("index").toInt();

  // Get the new gateway value from the request body
  String gatewayString = server.arg("gateway");
  IPAddress gateway;
  gateway.fromString(gatewayString);

  // Call the corresponding function to update the APConfig DHCPConfig gateway
  bool success = updateAPConfigDHCPConfigGatewayByIndex(index, gateway);

  // Create a JSON response with the success status
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["success"] = success;

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleAPConfigUpdatedhcp_config_subnet() {
  // Get the index from the URL parameter
  int index = server.pathArg("index").toInt();

  // Get the new subnet value from the request body
  String subnetString = server.arg("subnet");
  IPAddress subnet;
  subnet.fromString(subnetString);

  // Call the corresponding function to update the APConfig DHCPConfig subnet
  bool success = updateAPConfigDHCPConfigSubnetByIndex(index, subnet);

  // Create a JSON response with the success status
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["success"] = success;

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleDNSConfigPrimary_dnsGet() {
  // Get the index from the URL parameter
  int index = server.pathArg("index").toInt();

  // Call the corresponding function to get the APConfig DNSConfig primary DNS
  IPAddress primaryDNS = getAPConfigDNSConfigPrimaryDNSByIndex(index);

  // Create a JSON response with the primary DNS value
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["primary_dns"] = primaryDNS.toString();

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleDNSConfigSecondary_dnsGet() {
  // Get the index from the URL parameter
  int index = server.pathArg("index").toInt();

  // Call the corresponding function to get the APConfig DNSConfig secondary DNS
  IPAddress secondaryDNS = getAPConfigDNSConfigSecondaryDNSByIndex(index);

  // Create a JSON response with the secondary DNS value
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["secondary_dns"] = secondaryDNS.toString();

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleDNSConfigPrimary_dnsUpdate() {
  // Get the index from the URL parameter
  int index = server.pathArg("index").toInt();

  // Get the new primary DNS value from the request body
  String primaryDNSString = server.arg("primary_dns");
  IPAddress primaryDNS;
  primaryDNS.fromString(primaryDNSString);

  // Call the corresponding function to update the APConfig DNSConfig primary DNS
  bool success = updateAPConfigDNSConfigPrimaryDNSByIndex(index, primaryDNS);

  // Create a JSON response with the success status
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["success"] = success;

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleDNSConfigSecondary_dnsUpdate() {
  // Get the index from the URL parameter
  int index = server.pathArg("index").toInt();

  // Get the new secondary DNS value from the request body
  String secondaryDNSString = server.arg("secondary_dns");
  IPAddress secondaryDNS;
  secondaryDNS.fromString(secondaryDNSString);

  // Call the corresponding function to update the APConfig DNSConfig secondary DNS
  bool success = updateAPConfigDNSConfigSecondaryDNSByIndex(index, secondaryDNS);

  // Create a JSON response with the success status
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["success"] = success;

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleSoftAPConfigSSIDGet() {
  // Call the corresponding function to get the SoftAPConfig SSID
  String ssid = getSoftAPConfigssid();

  // Create a JSON response with the SSID value
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["SSID"] = ssid;

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleSoftAPConfigPasswordGet() {
  // Call the corresponding function to get the SoftAPConfig password
  String password = getSoftAPConfigPassword();

  // Create a JSON response with the password value
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["password"] = password;

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleSoftAPConfigChannelGet() {
  // Call the corresponding function to get the SoftAPConfig channel
  int channel = getSoftAPConfigChannel();

  // Create a JSON response with the channel value
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["channel"] = channel;

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleSoftAPConfigHiddenGet() {
  // Call the corresponding function to get the SoftAPConfig hidden status
  bool hidden = getSoftAPConfighidden();

  // Create a JSON response with the hidden status value
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["hidden"] = hidden;

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleSoftAPConfigMax_ConnectionGet() {
  // Call the corresponding function to get the SoftAPConfig max connection value
  int maxConnection = getSoftAPConfigmaxConnection();

  // Create a JSON response with the max connection value
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["max_connection"] = maxConnection;

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleSoftAPConfigSSIDUpdate() {
  // Get the new SSID from the request body
  String newSSID = server.arg("plain");

  // Call the corresponding function to update the SoftAPConfig SSID
  bool success = updateSoftAPConfigssid(newSSID);

  // Create a JSON response with the success status
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["success"] = success;

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleSoftAPConfigPasswordUpdate() {
  // Get the new password from the request body
  String newPassword = server.arg("plain");

  // Call the corresponding function to update the SoftAPConfig password
  bool success = updateSoftAPConfigPassword(newPassword);

  // Create a JSON response with the success status
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["success"] = success;

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleSoftAPConfigChannelUpdate() {
  // Get the new channel from the request body
  int newChannel = server.arg("plain").toInt();

  // Call the corresponding function to update the SoftAPConfig channel
  bool success = updateSoftAPConfigChannel(newChannel);

  // Create a JSON response with the success status
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["success"] = success;

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleSoftAPConfigHiddenUpdate() {
  // Get the new hidden status from the request body
  bool newHidden = server.arg("plain").toInt();

  // Call the corresponding function to update the SoftAPConfig hidden status
  bool success = updateSoftAPConfighidden(newHidden);

  // Create a JSON response with the success status
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["success"] = success;

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleSoftAPConfigMax_ConnectionUpdate() {
  // Get the new max connection value from the request body
  int newMaxConnection = server.arg("plain").toInt();

  // Call the corresponding function to update the SoftAPConfig max connection value
  bool success = updateSoftAPConfigmaxConnection(newMaxConnection);

  // Create a JSON response with the success status
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> jsonDoc;
  JsonObject jsonObject = jsonDoc.to<JsonObject>();
  jsonObject["success"] = success;

  // Send the JSON response
  String jsonResponse;
  serializeJson(jsonObject, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}
