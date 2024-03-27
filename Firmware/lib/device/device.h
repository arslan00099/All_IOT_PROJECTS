#ifndef DEVICE_H
#define DEVICE_H

#include <Arduino.h>
#include <config.pb.h>
#include "settings.h"

// Function to update the device name in the settings
bool updateDeviceName(const String& deviceName);

// Function to read the device name from the settings
String readDeviceName();

#endif
