#ifndef API_SERVER_H
#define API_SERVER_H

#include <Arduino.h>
//#include <ESPAsyncWebServer.h>

// Declare the AsyncWebServer object
extern AsyncWebServer server;

// Function to handle API requests
void handleAPIRequest(AsyncWebServerRequest *request);

// Function to start the API server
void startAPIServer();

// Endpoint handlers for NTPConfig
void handleNTPConfigGet();

// Endpoint handlers for DeviceName
void handleDeviceNameGet();
void handleDeviceNameUpdate();

// Endpoint handlers for BootSequence
void handleBootSequenceGet();
void handleBootSequenceUpdate();

// Endpoint handlers for MQTTConfig
void handleMQTTConfigGet();
void handleMQTTConfigUpdate();

// Endpoint handlers for MQTTConfig - Individual fields
void handleMQTTConfigHostUpdate();
void handleMQTTConfigPortUpdate();
void handleMQTTConfigKeepAliveUpdate();
void handleMQTTConfigCleanSessionUpdate();
void handleMQTTConfigTimeoutUpdate();

// Endpoint handlers for MQTTConfig - Topics
void handleMQTTConfigPublishTopicsList();
void handleMQTTConfigSubscribeTopicsList();
void handleMQTTConfigPublishTopicAdd();
void handleMQTTConfigSubscribeTopicAdd();
void handleMQTTConfigPublishTopicDelete();
void handleMQTTConfigSubscribeTopicDelete();

// Endpoint handlers for SoftAPConfig
void handleSoftAPConfigGet();
void handleSoftAPConfigUpdate();
void handleSoftAPConfigSSIDGet();
void handleSoftAPConfigPasswordGet();
void handleSoftAPConfigChannelGet();
void handleSoftAPConfigHiddenGet();
void handleSoftAPConfigMax_ConnectionGet();
void handleSoftAPConfigSSIDUpdate();
void handleSoftAPConfigPasswordUpdate();
void handleSoftAPConfigChannelUpdate();
void handleSoftAPConfigHiddenUpdate();
void handleSoftAPConfigMax_ConnectionUpdate();

// Endpoint handlers for AP Configurations
void handleApconfigsList();
void handleApconfigsGet();
void handleApconfigsDelete();
void handleApconfigUpdate();

// Endpoint handlers for WiFi Configurations
void handleAPConfigSSIDGet();
void handleAPConfigPasswordGet();
void handleAPConfigChannelGet();
void handleAPConfigHiddenGet();
void handleAPConfigMax_ConnectionGet();
void handleAPConfigSSIDUpdate();
void handleAPConfigPasswordUpdate();
void handleAPConfigChannelUpdate();
void handleAPConfigHiddenUpdate();
void handleAPConfigMax_ConnectionUpdate();
void handleAPConfigGetdhcp_config_ip();
void handleAPConfigGetdhcp_config_gateway();
void handleAPConfigGetdhcp_config_subnet();
void handleAPConfigUpdatedhcp_config_ip();
void handleAPConfigUpdatedhcp_config_gateway();
void handleAPConfigUpdatedhcp_config_subnet();
void handleDNSConfigGet();
void handleDNSConfigUpdate();
void handleDNSConfigDelete();
void handleDNSConfigGetPrimaryDNS();
void handleDNSConfigUpdatePrimaryDNS();
void handleDNSConfigGetSecondaryDNS();
void handleDNSConfigUpdateSecondaryDNS();
void handleDNSConfigPrimary_dnsGet();
void handleDNSConfigSecondary_dnsGet();
void handleDNSConfigPrimary_dnsUpdate();
void handleDNSConfigSecondary_dnsUpdate();
void handleDHCPConfigGet();
void handleDHCPConfigUpdate();
void handleDHCPConfigDelete();

#endif
