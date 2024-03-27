#ifndef SETTINGS_FLAGS_H
#define SETTINGS_FLAGS_H

#include "settings.h"

// Function to check if custom mDNS is enabled
bool is_custom_mdns_enabled();

// Function to check if custom DHCP is enabled
bool is_custom_dhcp_enabled();

// Function to check if custom NTP is enabled
bool is_custom_ntp_enabled();

// Function to check if MQTT is enabled
bool is_mqtt_enabled();

// Function to enable/disable custom mDNS
bool custom_mdns_enabled(bool enable);

// Function to enable/disable custom DHCP
bool custom_dhcp_enabled(bool enable);

// Function to enable/disable custom NTP
bool custom_ntp_enabled(bool enable);

// Function to enable/disable MQTT
bool mqtt_enabled(bool enable);

#endif
