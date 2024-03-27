#include "softap_config.h"

config::SoftAPConfig getSoftAPConfig() {
  config::Settings settings;
  if (!loadSettings(settings) || !settings.has_soft_ap_config()) {
    return config::SoftAPConfig();
  }
  return settings.soft_ap_config();
}

bool updateSoftAPConfigPassword(const String& ssid, const String& password) {
  config::SoftAPConfig softAPConfig;
  if (!readSoftAPConfig(softAPConfig)) {
    return false;
  }
  softAPConfig.set_ssid(ssid);
  softAPConfig.set_password(password);
  return saveSettings(softAPConfig);
}

bool updateSoftAPConfig(const String& ssid, const String& password, const String& psk,
                        int channel, bool hidden, int maxConnection) {
  config::SoftAPConfig softAPConfig;
  if (!readSoftAPConfig(softAPConfig)) {
    return false;
  }
  softAPConfig.set_ssid(ssid);
  softAPConfig.set_password(password);
  softAPConfig.set_psk(psk);
  softAPConfig.set_channel(channel);
  softAPConfig.set_hidden(hidden);
  softAPConfig.set_max_connection(maxConnection);
  return saveSettings(softAPConfig);
}

bool updateSoftAPConfigssid(const String& ssid) {
  config::SoftAPConfig softAPConfig;
  if (!readSoftAPConfig(softAPConfig)) {
    return false;
  }
  softAPConfig.set_ssid(ssid);
  return saveSettings(softAPConfig);
}

bool updateSoftAPConfigPassword(const String& password) {
  config::SoftAPConfig softAPConfig;
  if (!readSoftAPConfig(softAPConfig)) {
    return false;
  }
  softAPConfig.set_password(password);
  return saveSettings(softAPConfig);
}

bool updateSoftAPConfigPSK(const String& psk) {
  config::SoftAPConfig softAPConfig;
  if (!readSoftAPConfig(softAPConfig)) {
    return false;
  }
  softAPConfig.set_psk(psk);
  return saveSettings(softAPConfig);
}

bool updateSoftAPConfigChannel(int channel) {
  config::SoftAPConfig softAPConfig;
  if (!readSoftAPConfig(softAPConfig)) {
    return false;
  }
  softAPConfig.set_channel(channel);
  return saveSettings(softAPConfig);
}

bool updateSoftAPConfighidden(bool hidden) {
  config::SoftAPConfig softAPConfig;
  if (!readSoftAPConfig(softAPConfig)) {
    return false;
  }
  softAPConfig.set_hidden(hidden);
  return saveSettings(softAPConfig);
}

bool updateSoftAPConfigmaxConnection(int maxConnection) {
  config::SoftAPConfig softAPConfig;
  if (!readSoftAPConfig(softAPConfig)) {
    return false;
  }
  softAPConfig.set_max_connection(maxConnection);
  return saveSettings(softAPConfig);
}

bool deleteSoftAPConfigPassword() {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }

  config::SoftAPConfig* softAPConfig = settings.mutable_soft_ap_config();
  softAPConfig->clear_password();

  return saveSettings(settings);
}

bool deleteSoftAPConfigPSK() {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }

  config::SoftAPConfig* softAPConfig = settings.mutable_soft_ap_config();
  softAPConfig->clear_psk();

  return saveSettings(settings);
}

bool deleteSoftAPConfigChannel() {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }

  config::SoftAPConfig* softAPConfig = settings.mutable_soft_ap_config();
  softAPConfig->clear_channel();

  return saveSettings(settings);
}

bool deleteSoftAPConfighidden() {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }

  config::SoftAPConfig* softAPConfig = settings.mutable_soft_ap_config();
  softAPConfig->clear_hidden();

  return saveSettings(settings);
}

bool deleteSoftAPConfigmaxConnection() {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }

  config::SoftAPConfig* softAPConfig = settings.mutable_soft_ap_config();
  softAPConfig->clear_max_connection();

  return saveSettings(settings);
}

String getSoftAPConfigssid() {
  config::Settings settings;
  if (!loadSettings(settings) || !settings.has_soft_ap_config()) {
    return "";
  }
  const config::SoftAPConfig& softAPConfig = settings.soft_ap_config();
  return softAPConfig.ssid();
}

String getSoftAPConfigPassword() {
  config::Settings settings;
  if (!loadSettings(settings) || !settings.has_soft_ap_config()) {
    return "";
  }
  const config::SoftAPConfig& softAPConfig = settings.soft_ap_config();
  return softAPConfig.password();
}

String getSoftAPConfigPSK() {
  config::Settings settings;
  if (!loadSettings(settings) || !settings.has_soft_ap_config()) {
    return "";
  }
  const config::SoftAPConfig& softAPConfig = settings.soft_ap_config();
  return softAPConfig.psk();
}

int getSoftAPConfigChannel() {
  config::Settings settings;
  if (!loadSettings(settings) || !settings.has_soft_ap_config()) {
    return 0;
  }
  const config::SoftAPConfig& softAPConfig = settings.soft_ap_config();
  return softAPConfig.channel();
}

bool getSoftAPConfighidden() {
  config::Settings settings;
  if (!loadSettings(settings) || !settings.has_soft_ap_config()) {
    return false;
  }
  const config::SoftAPConfig& softAPConfig = settings.soft_ap_config();
  return softAPConfig.hidden();
}

int getSoftAPConfigmaxConnection() {
  config::Settings settings;
  if (!loadSettings(settings) || !settings.has_soft_ap_config()) {
    return 0;
  }
  const config::SoftAPConfig& softAPConfig = settings.soft_ap_config();
  return softAPConfig.max_connection();
}
