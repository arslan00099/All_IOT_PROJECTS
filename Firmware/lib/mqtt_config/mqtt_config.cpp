#include "mqtt_config.h"

bool deleteMQTTConfig() {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }
  settings.clear_mqtt_config();
  return saveSettings(settings);
}

bool readMQTTConfig(config::MQTTConfig& mqttConfig) {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }
  if (!settings.has_mqtt_config()) {
    return false;
  }
  mqttConfig = settings.mqtt_config();
  return true;
}

bool updateMQTTConfighost(const String& host) {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }
  config::MQTTConfig* mqttConfig = settings.mutable_mqtt_config();
  mqttConfig->set_host(host);
  return saveSettings(settings);
}

bool updateMQTTConfigport(int port) {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }
  config::MQTTConfig* mqttConfig = settings.mutable_mqtt_config();
  mqttConfig->set_port(port);
  return saveSettings(settings);
}

bool updateMQTTConfigkeep_alive(bool keepAlive) {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }
  config::MQTTConfig* mqttConfig = settings.mutable_mqtt_config();
  mqttConfig->mutable_keep_alive()->set_keep_alive(keepAlive);
  return saveSettings(settings);
}

bool updateMQTTConfigclean_session(bool cleanSession) {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }
  config::MQTTConfig* mqttConfig = settings.mutable_mqtt_config();
  mqttConfig->mutable_clean_session()->set_clean_session(cleanSession);
  return saveSettings(settings);
}

bool updateMQTTConfig(int timeout) {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }
  config::MQTTConfig* mqttConfig = settings.mutable_mqtt_config();
  mqttConfig->set_timeout(timeout);
  return saveSettings(settings);
}

int countMQTTConfigpublish_topics() {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return 0;
  }
  if (!settings.has_mqtt_config()) {
    return 0;
  }
  return settings.mqtt_config().publish_topics().topic_size();
}

int countMQTTConfigsubscribe_topics() {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return 0;
  }
  if (!settings.has_mqtt_config()) {
    return 0;
  }
  return settings.mqtt_config().subscribe_topics().topic_size();
}

bool updateMQTTConfigpublish_topicByIndex(int index, const String& topic) {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }
  if (!settings.has_mqtt_config()) {
    return false;
  }
  config::MQTTConfig* mqttConfig = settings.mutable_mqtt_config();
  if (index >= 0 && index < mqttConfig->mutable_publish_topics()->topic_size()) {
    mqttConfig->mutable_publish_topics()->set_topic(index, topic);
    return saveSettings(settings);
  }
  return false;
}

bool updateMQTTConfigsubscribe_topicByIndex(int index, const String& topic) {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }
  if (!settings.has_mqtt_config()) {
    return false;
  }
  config::MQTTConfig* mqttConfig = settings.mutable_mqtt_config();
  if (index >= 0 && index < mqttConfig->mutable_subscribe_topics()->topic_size()) {
    mqttConfig->mutable_subscribe_topics()->set_topic(index, topic);
    return saveSettings(settings);
  }
  return false;
}

bool deleteMQTTConfigpublish_topicByIndex(int index) {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }
  if (!settings.has_mqtt_config()) {
    return false;
  }
  config::MQTTConfig* mqttConfig = settings.mutable_mqtt_config();
  if (index >= 0 && index < mqttConfig->mutable_publish_topics()->topic_size()) {
    mqttConfig->mutable_publish_topics()->mutable_topic()->erase(mqttConfig->mutable_publish_topics()->mutable_topic()->begin() + index);
    return saveSettings(settings);
  }
  return false;
}

bool deleteMQTTConfigsubscribe_topicByIndex(int index) {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }
  if (!settings.has_mqtt_config()) {
    return false;
  }
  config::MQTTConfig* mqttConfig = settings.mutable_mqtt_config();
  if (index >= 0 && index < mqttConfig->mutable_subscribe_topics()->topic_size()) {
    mqttConfig->mutable_subscribe_topics()->mutable_topic()->erase(mqttConfig->mutable_subscribe_topics()->mutable_topic()->begin() + index);
    return saveSettings(settings);
  }
  return false;
}

bool addMQTTConfigpublish_topic(const String& topic) {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }
  config::MQTTConfig* mqttConfig = settings.mutable_mqtt_config();
  mqttConfig->mutable_publish_topics()->add_topic(topic);
  return saveSettings(settings);
}

bool addMQTTConfigsubscribe_topic(const String& topic) {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return false;
  }
  config::MQTTConfig* mqttConfig = settings.mutable_mqtt_config();
  mqttConfig->mutable_subscribe_topics()->add_topic(topic);
  return saveSettings(settings);
}

String getMQTTConfigpublish_topicByIndex(int index) {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return "";
  }
  if (!settings.has_mqtt_config()) {
    return "";
  }
  const config::MQTTConfig& mqttConfig = settings.mqtt_config();
  if (index >= 0 && index < mqttConfig.publish_topics().topic_size()) {
    return mqttConfig.publish_topics().topic(index);
  }
  return "";
}

String getMQTTConfigsubscribe_topicByIndex(int index) {
  config::Settings settings;
  if (!loadSettings(settings)) {
    return "";
  }
  if (!settings.has_mqtt_config()) {
    return "";
  }
  const config::MQTTConfig& mqttConfig = settings.mqtt_config();
  if (index >= 0 && index < mqttConfig.subscribe_topics().topic_size()) {
    return mqttConfig.subscribe_topics().topic(index);
  }
  return "";
}
