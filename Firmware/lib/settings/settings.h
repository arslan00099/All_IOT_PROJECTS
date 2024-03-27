#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <settings.pb.h>
#include <LittleFS.h>

// Function to load the device settings from the settings.bin file
bool loadSettings(Settings& settings);

// Function to save the device settings to the settings.bin file
bool saveSettings(const Settings& settings);

// Function to delete the settings.bin file
void deleteSettings();

// Function to create default settings if the settings.bin file doesn't exist
void createDefaultSettings();


bool encode_setting(pb_ostream_t *stream, const pb_msgdesc_t *field, void * setting);
bool encode_string(pb_ostream_t* stream, const pb_field_t* field, void* const* arg);


#endif
