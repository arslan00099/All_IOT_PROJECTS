#include <settings.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include <pb_common.h>

bool encode_string(pb_ostream_t* stream, const pb_field_t* field, void* const* arg)
{
    const char* str = (const char*)(*arg);

    if (!pb_encode_tag_for_field(stream, field))
        return false;

    return pb_encode_string(stream, (uint8_t*)str, strlen(str));
}

bool encode_setting(pb_ostream_t* stream, const pb_field_t* field, void* const* arg)
{
    const pb_msgdesc_t* submsg_desc = (const pb_msgdesc_t*)field->submsg_desc;
    void* submsg = (void*)(*arg);

    if (!pb_encode_tag_for_field(stream, field))
        return false;

    return pb_encode_submessage(stream, submsg_desc, submsg);
}

bool loadSettings(Settings& settings)
{
    File configFile = LittleFS.open("/settings.bin", "r");
    if (!configFile)
    { 
        Serial.print("ERR: No Config");
        return false;
    }

    uint8_t buffer[512];
    size_t file_size = configFile.size();

    if (file_size > sizeof(buffer))
    {
        Serial.print("ERR: File too big");
        configFile.close();
        return false;  // File too large for buffer
    }

    configFile.read(buffer, file_size);
    configFile.close();

    pb_istream_t in_stream = pb_istream_from_buffer(buffer, file_size);
    bool status = pb_decode(&in_stream, Settings_fields, &settings);

    return status;
}

bool saveSettings(const Settings& settings)
{
    File configFile = LittleFS.open("/settings.bin", "w");
    if (!configFile)
    {
        return false;
    }

    uint8_t buffer[512];
    pb_ostream_t out_stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    bool status = pb_encode(&out_stream, Settings_fields, &settings);

    if (status)
    {
        configFile.write(buffer, out_stream.bytes_written);
        configFile.close();
    }
    else
    {
        configFile.close();
        return false;
    }

    return true;
}

void deleteSettings()
{
    LittleFS.remove("/settings.bin");
}

void createDefaultSettings()
{
    Settings settings = Settings_init_zero;

    char device_name[17] = "ck9_generic_0001";
    char ssid[10] = "k9net.org";

    settings.boot_sequence = BootSequence_BOOT_SEQUENCE_AP_ONLY;

    // Set default device name
    settings.device_name.arg = &device_name;
    settings.device_name.funcs.encode = &encode_string;
/* 
    // Set default SoftAP configuration, password = ssid
    settings.soft_ap_config.SSID.arg = &ssid;
    settings.soft_ap_config.SSID.funcs.encode = &encode_string;

    // Set default SoftAP configuration, password = ssid
    settings.soft_ap_config.password.arg = &ssid;
    settings.soft_ap_config.password.funcs.encode = &encode_string;

    // Set default flags for custom options
    settings.enable_custom_mdns = false;
    settings.enable_custom_dhcp = false;
    settings.enable_custom_ntp = false;
    settings.enable_mqtt = false;
    settings.enable_esp_now = false;
    settings.enable_websocket = false;
    settings.enable_api_server = false;
 */
    if (saveSettings(settings))
    {
        Serial.println("Settings created and saved successfully.");
    }
    else
    {
        Serial.println("Failed to save settings.");
    }
}