#ifndef MQTT_CONFIG_H
#define MQTT_CONFIG_H

#include <Arduino.h>
#include "include/settings.pb.h"
#include "include/settings.h"

bool deleteMQTTConfig();
bool readMQTTConfig(config::MQTTConfig& MQTTConfig);
bool updateMQTTConfighost(String& host);
bool updateMQTTConfigport (int& port);
bool updateMQTTConfigkeep_alive (bool& keep_alive);
bool updateMQTTConfigclean_session(bool& clean_session);
bool updateMQTTConfig(int& timeout);

int countMQTTConfigpublish_topics();
int countMQTTConfigsubscribe_topics();

bool updateMQTTConfigpublish_topicByIndex(int index, String& topic);
bool updateMQTTConfigsubscribe_topicByIndex(int index, String& topic);

bool deleteMQTTConfigpublish_topicByIndex(int index);
bool deleteMQTTConfigsubscribe_topicByIndex(int index);

bool addMQTTConfigpublish_topic(String& topic);
bool addMQTTConfigsubscribe_topic(String& topic);

String getMQTTConfigpublish_topicByIndex(int index);
String getMQTTConfigsubscribe_topicByIndex(int index);

#endif
 
