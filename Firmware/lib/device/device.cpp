#include "device.h"

bool updateDeviceName(const String& deviceName) {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }
  settings.set_device_name(deviceName);
  return saveSettings(settings);
}

String readDeviceName() {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return "";
  }
  return settings.device_name();
}
