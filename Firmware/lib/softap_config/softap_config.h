#ifndef SOFTAP_CONFIG_H
#define SOFTAP_CONFIG_H

#include <Arduino.h>
#include "settings.pb.h"
#include "settings.h"

// Function to get the SoftAP configuration
SoftAPConfig getSoftAPConfig();

// Function to update the SoftAP configuration with password
bool updateSoftAPConfigPassword(const String& ssid, const String& password);

// Function to update the SoftAP configuration
bool updateSoftAPConfig(const String& ssid, const String& password, const String& psk,
                        int channel, bool hidden, int maxConnection);

// Function to update the SoftAP configuration - SSID only
bool updateSoftAPConfigSSID(const String& ssid);

// Function to update the SoftAP configuration - password only
bool updateSoftAPConfigPassword(const String& password);

// Function to update the SoftAP configuration - PSK only
bool updateSoftAPConfigPSK(const String& psk);

// Function to update the SoftAP configuration - channel only
bool updateSoftAPConfigChannel(int channel);

// Function to update the SoftAP configuration - hidden flag only
bool updateSoftAPConfigHidden(bool hidden);

// Function to update the SoftAP configuration - max connection count only
bool updateSoftAPConfigMaxConnection(int maxConnection);

// Function to get the SoftAP configuration - SSID
String getSoftAPConfigSSID();

// Function to get the SoftAP configuration - password
String getSoftAPConfigPassword();

// Function to get the SoftAP configuration - PSK
String getSoftAPConfigPSK();

// Function to get the SoftAP configuration - channel
int getSoftAPConfigChannel();

// Function to get the SoftAP configuration - hidden flag
bool getSoftAPConfigHidden();

// Function to get the SoftAP configuration - max connection count
int getSoftAPConfigMaxConnection();

// Function to delete the SoftAP configuration - password
bool deleteSoftAPConfigPassword();

// Function to delete the SoftAP configuration - PSK
bool deleteSoftAPConfigPSK();

// Function to delete the SoftAP configuration - channel
bool deleteSoftAPConfigChannel();

// Function to delete the SoftAP configuration - hidden flag
bool deleteSoftAPConfigHidden();

// Function to delete the SoftAP configuration - max connection count
bool deleteSoftAPConfigMaxConnection();

#endif
