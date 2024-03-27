#include "settings_flags.h"

bool is_custom_mdns_enabled() {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }
  return settings.enable_custom_mdns();
}

bool is_custom_dhcp_enabled() {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }
  return settings.enable_custom_dhcp();
}

bool is_custom_ntp_enabled() {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }
  return settings.enable_custom_ntp();
}

bool is_mqtt_enabled() {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }
  return settings.enable_mqtt();
}

bool custom_mdns_enabled(bool enable) {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }
  settings.set_enable_custom_mdns(enable);
  return saveSettings(settings);
}

bool custom_dhcp_enabled(bool enable) {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }
  settings.set_enable_custom_dhcp(enable);
  return saveSettings(settings);
}

bool custom_ntp_enabled(bool enable) {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }
  settings.set_enable_custom_ntp(enable);
  return saveSettings(settings);
}

bool mqtt_enabled(bool enable) {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }
  settings.set_enable_mqtt(enable);
  return saveSettings(settings);
}
 
