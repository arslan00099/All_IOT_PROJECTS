bool updateAPConfigPasswordByIndex(int index, const String& password) {
  config::APConfig apConfig;
  if (!readAPConfigByIndex(index, apConfig)) {
    return false;
  }
  apConfig.set_password(password);
  return saveAPConfigByIndex(index, apConfig);
}

bool updateAPConfigChannelByIndex(int index, int channel) {
  config::APConfig apConfig;
  if (!readAPConfigByIndex(index, apConfig)) {
    return false;
  }
  apConfig.set_channel(channel);
  return saveAPConfigByIndex(index, apConfig);
}

bool updateAPConfigHiddenByIndex(int index, bool hidden) {
  config::APConfig apConfig;
  if (!readAPConfigByIndex(index, apConfig)) {
    return false;
  }
  apConfig.set_hidden(hidden);
  return saveAPConfigByIndex(index, apConfig);
}

bool updateAPConfigMaxConnectionByIndex(int index, int maxConnection) {
  config::APConfig apConfig;
  if (!readAPConfigByIndex(index, apConfig)) {
    return false;
  }
  apConfig.set_max_connection(maxConnection);
  return saveAPConfigByIndex(index, apConfig);
}

bool updateAPConfigDNSConfigPrimaryDNSByIndex(int index, const config::IPAddress& primaryDNS) {
  config::APConfig apConfig;
  if (!readAPConfigByIndex(index, apConfig)) {
    return false;
  }
  if (!apConfig.has_dns_config()) {
    return false;
  }
  apConfig.mutable_dns_config()->set_primary_dns(primaryDNS);
  return saveAPConfigByIndex(index, apConfig);
}

bool updateAPConfigDNSConfigSecondaryDNSByIndex(int index, const config::IPAddress& secondaryDNS) {
  config::APConfig apConfig;
  if (!readAPConfigByIndex(index, apConfig)) {
    return false;
  }
  if (!apConfig.has_dns_config()) {
    return false;
  }
  apConfig.mutable_dns_config()->set_secondary_dns(secondaryDNS);
  return saveAPConfigByIndex(index, apConfig);
}

bool updateAPConfigDHCPConfigIPByIndex(int index, const config::IPAddress& ip) {
  config::APConfig apConfig;
  if (!readAPConfigByIndex(index, apConfig)) {
    return false;
  }
  if (!apConfig.has_dhcp_config()) {
    return false;
  }
  apConfig.mutable_dhcp_config()->set_ip(ip);
  return saveAPConfigByIndex(index, apConfig);
}

bool updateAPConfigDHCPConfigSubnetByIndex(int index, const config::IPAddress& subnet) {
  config::APConfig apConfig;
  if (!readAPConfigByIndex(index, apConfig)) {
    return false;
  }
  if (!apConfig.has_dhcp_config()) {
    return false;
  }
  apConfig.mutable_dhcp_config()->set_subnet(subnet);
  return saveAPConfigByIndex(index, apConfig);
}

bool updateAPConfigDHCPConfigGatewayByIndex(int index, const config::IPAddress& gateway) {
  config::APConfig apConfig;
  if (!readAPConfigByIndex(index, apConfig)) {
    return false;
  }
  if (!apConfig.has_dhcp_config()) {
    return false;
  }
  apConfig.mutable_dhcp_config()->set_gateway(gateway);
  return saveAPConfigByIndex(index, apConfig);
}

String getAPConfigPasswordByIndex(int index) {
  config::APConfig apConfig;
  if (!readAPConfigByIndex(index, apConfig)) {
    return "";
  }
  return apConfig.password();
}

int getAPConfigChannelByIndex(int index) {
  config::APConfig apConfig;
  if (!readAPConfigByIndex(index, apConfig)) {
    return -1;
  }
  return apConfig.channel();
}

bool getAPConfigHiddenByIndex(int index) {
  config::APConfig apConfig;
  if (!readAPConfigByIndex(index, apConfig)) {
    return false;
  }
  return apConfig.hidden();
}

int getAPConfigMaxConnectionByIndex(int index) {
  config::APConfig apConfig;
  if (!readAPConfigByIndex(index, apConfig)) {
    return -1;
  }
  return apConfig.max_connection();
}

IPAddress getAPConfigDNSConfigPrimaryDNSByIndex(int index) {
  config::APConfig apConfig;
  if (!readAPConfigByIndex(index, apConfig)) {
    return IPAddress();
  }
  if (!apConfig.has_dns_config()) {
    return IPAddress();
  }
  return apConfig.dns_config().primary_dns();
}

IPAddress getAPConfigDNSConfigSecondaryDNSByIndex(int index) {
  config::APConfig apConfig;
  if (!readAPConfigByIndex(index, apConfig)) {
    return IPAddress();
  }
  if (!apConfig.has_dns_config()) {
    return IPAddress();
  }
  return apConfig.dns_config().secondary_dns();
}

IPAddress getAPConfigDHCPConfigIPByIndex(int index) {
  config::APConfig apConfig;
  if (!readAPConfigByIndex(index, apConfig)) {
    return IPAddress();
  }
  if (!apConfig.has_dhcp_config()) {
    return IPAddress();
  }
  return apConfig.dhcp_config().ip();
}

IPAddress getAPConfigDHCPConfigSubnetByIndex(int index) {
  config::APConfig apConfig;
  if (!readAPConfigByIndex(index, apConfig)) {
    return IPAddress();
  }
  if (!apConfig.has_dhcp_config()) {
    return IPAddress();
  }
  return apConfig.dhcp_config().subnet();
}

IPAddress getAPConfigDHCPConfigGatewayByIndex(int index) {
  config::APConfig apConfig;
  if (!readAPConfigByIndex(index, apConfig)) {
    return IPAddress();
  }
  if (!apConfig.has_dhcp_config()) {
    return IPAddress();
  }
  return apConfig.dhcp_config().gateway();
}

int countAPConfigs() {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return 0;
  }
  if (!settings.has_wifi_config()) {
    return 0;
  }
  return settings.wifi_config().ap_config_size();
} 

#include "ap_config.h"

void listAPConfigs() {
  config::Settings settings;
  if (!loadSettings(settings) || !settings.has_wifi_config()) {
    return;
  }
  const config::WiFiConfig& wifiConfig = settings.wifi_config();
  for (int i = 0; i < wifiConfig.ap_config_size(); ++i) {
    const config::APConfig& apConfig = wifiConfig.ap_config(i);
    Serial.print("AP Config ");
    Serial.print(i);
    Serial.print(": SSID=");
    Serial.print(apConfig.ssid());
    Serial.print(", Password=");
    Serial.print(apConfig.password());
    Serial.print(", Channel=");
    Serial.print(apConfig.channel());
    Serial.print(", Hidden=");
    Serial.print(apConfig.hidden() ? "Yes" : "No");
    Serial.print(", Max Connection=");
    Serial.println(apConfig.max_connection());
  }
}

bool readAPConfigByIndex(int index, config::APConfig& apConfig) {
  config::Settings settings;
  if (!loadSettings(settings) || !settings.has_wifi_config()) {
    return false;
  }
  const config::WiFiConfig& wifiConfig = settings.wifi_config();
  if (index < 0 || index >= wifiConfig.ap_config_size()) {
    return false;
  }
  apConfig = wifiConfig.ap_config(index);
  return true;
}

bool deleteAPConfigByIndex(int index) {
  config::Settings settings;
  if (!loadSettings(settings) || !settings.has_wifi_config()) {
    return false;
  }
  config::WiFiConfig* wifiConfig = settings.mutable_wifi_config();
  if (index < 0 || index >= wifiConfig->ap_config_size()) {
    return false;
  }
  wifiConfig->mutable_ap_config()->erase(wifiConfig->ap_config().begin() + index);
  return saveSettings(settings);
}

String getAPConfigSSIDByIndex(int index) {
  String ssid;

  // Read the APConfig by index
  config::APConfig apConfig;
  if (readAPConfigByIndex(index, apConfig)) {
    ssid = apConfig.ssid();
  }

  return ssid;
}
