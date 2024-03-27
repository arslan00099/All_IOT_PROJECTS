#ifndef AP_CONFIG_H
#define AP_CONFIG_H

#include <Arduino.h>
#include "settings.pb.h"
#include "settings.h"

// Function to update AP configuration password by index
bool updateAPConfigPasswordByIndex(int index, const String& password);

// Function to update AP configuration channel by index
bool updateAPConfigChannelByIndex(int index, int channel);

// Function to update AP configuration hidden status by index
bool updateAPConfigHiddenByIndex(int index, bool hidden);

// Function to update AP configuration maximum connection count by index
bool updateAPConfigMaxConnectionByIndex(int index, int maxConnection);

// Function to update AP configuration primary DNS by index
bool updateAPConfigDNSConfigPrimaryDNSByIndex(int index, const config::IPAddress& primaryDNS);

// Function to update AP configuration secondary DNS by index
bool updateAPConfigDNSConfigSecondaryDNSByIndex(int index, const config::IPAddress& secondaryDNS);

// Function to update AP configuration IP address by index
bool updateAPConfigDHCPConfigIPByIndex(int index, const config::IPAddress& ip);

// Function to update AP configuration subnet mask by index
bool updateAPConfigDHCPConfigSubnetByIndex(int index, const config::IPAddress& subnet);

// Function to update AP configuration gateway by index
bool updateAPConfigDHCPConfigGatewayByIndex(int index, const config::IPAddress& gateway);

// Function to get AP configuration SSID by index
String getAPConfigSSIDByIndex(int index);

// Function to get AP configuration password by index
String getAPConfigPasswordByIndex(int index);

// Function to get AP configuration channel by index
int getAPConfigChannelByIndex(int index);

// Function to check if AP configuration is hidden by index
bool getAPConfigHiddenByIndex(int index);

// Function to get AP configuration maximum connection count by index
int getAPConfigMaxConnectionByIndex(int index);

// Function to get AP configuration primary DNS by index
IPAddress getAPConfigDNSConfigPrimaryDNSByIndex(int index);

// Function to get AP configuration secondary DNS by index
IPAddress getAPConfigDNSConfigSecondaryDNSByIndex(int index);

// Function to get AP configuration IP address by index
IPAddress getAPConfigDHCPConfigIPByIndex(int index);

// Function to get AP configuration subnet mask by index
IPAddress getAPConfigDHCPConfigSubnetByIndex(int index);

// Function to get AP configuration gateway by index
IPAddress getAPConfigDHCPConfigGatewayByIndex(int index);

// Function to count the number of AP configurations
int countAPConfigs();

// Function to list all AP configurations
void listAPConfigs();

// Function to read AP configuration by index
bool readAPConfigByIndex(int index, config::APConfig& apConfig);

// Function to delete AP configuration by index
bool deleteAPConfigByIndex(int index);

#endif

