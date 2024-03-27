/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.8-dev */

#ifndef SETTINGS_PB_H_INCLUDED
#define SETTINGS_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
/* Enumeration for BootSequence options */
typedef enum _BootSequence {
    BootSequence_BOOT_SEQUENCE_UNKNWON = 0,
    /* Boot in Access Point (AP) mode only */
    BootSequence_BOOT_SEQUENCE_AP_ONLY = 1,
    /* Boot in Station mode, synchronize with an Access Point (AP), then switch back to Access Point (AP) mode */
    BootSequence_BOOT_SEQUENCE_STATION_SYNC_AP = 2,
    /* Boot in Station mode and fall back to Access Point (AP) mode if no AP is available */
    BootSequence_BOOT_SEQUENCE_STATION_FALLBACK_AP = 3
} BootSequence;

/* Struct definitions */
/* Message to represent MQTT Topics */
typedef struct _MQTTTopics {
    pb_callback_t topic;
} MQTTTopics;

/* Message to represent MQTT KeepAlive configuration */
typedef struct _MQTTKeepAlive {
    int32_t keep_alive;
} MQTTKeepAlive;

/* Message to represent MQTT CleanSession configuration */
typedef struct _MQTTCleanSession {
    bool cleanSession;
} MQTTCleanSession;

/* Message to represent MQTT configuration options */
typedef struct _MQTTConfig {
    pb_callback_t host;
    bool has_port;
    int32_t port;
    bool has_publish_topics;
    MQTTTopics publish_topics;
    bool has_subscribe_topics;
    MQTTTopics subscribe_topics;
    bool has_keep_alive;
    MQTTKeepAlive keep_alive;
    bool has_clean_session;
    MQTTCleanSession clean_session;
    bool has_timeout;
    int32_t timeout;
} MQTTConfig;

/* Message to represent mDNS (Multicast DNS) configuration for ESP32 */
typedef struct _MDNSConfig {
    /* Configuration for ESP32 mDNS */
    pb_callback_t device_name;
    pb_callback_t service_type;
    bool has_port;
    int32_t port;
    bool has_advertise_ip;
    bool advertise_ip;
} MDNSConfig;

/* Message to represent DNS configuration */
typedef struct _DNSConfig {
    pb_callback_t primary_dns;
    pb_callback_t secondary_dns;
} DNSConfig;

/* Message to represent DHCP configuration */
typedef struct _DHCPConfig {
    /* One of - Automatic or Static config */
    pb_callback_t ip;
    pb_callback_t gateway;
    pb_callback_t subnet;
} DHCPConfig;

/* Possible configuration options for setting up the ESP in SoftAP mode */
typedef struct _SoftAPConfig {
    pb_callback_t SSID;
    pb_callback_t password;
    pb_callback_t psk;
    bool has_channel;
    int32_t channel;
    bool has_hidden;
    bool hidden;
    bool has_max_connection;
    int32_t max_connection;
    bool has_dhcp_config;
    DHCPConfig dhcp_config;
    bool has_mdns_config;
    MDNSConfig mdns_config;
} SoftAPConfig;

/* Message to represent NTP (Network Time Protocol) configuration */
typedef struct _NTPConfig {
    pb_callback_t pool_server_name;
    int32_t port;
} NTPConfig;

/* Message to represent Access Point (AP) configuration */
typedef struct _APConfig {
    pb_callback_t SSID;
    pb_callback_t password;
    bool has_channel;
    int32_t channel;
    bool has_hidden;
    bool hidden;
    bool has_max_connection;
    int32_t max_connection;
    bool has_dns_config;
    DNSConfig dns_config;
    bool has_dhcp_config;
    DHCPConfig dhcp_config;
    bool has_mdns_config;
    MDNSConfig mdns_config;
} APConfig;

/* Message to represent Wi-Fi configuration options */
typedef struct _WiFiConfig {
    pb_callback_t ap_config;
} WiFiConfig;

/* Message to represent device settings and configurations */
typedef struct _Settings {
    /* Required fields */
    pb_callback_t device_name;
    BootSequence boot_sequence;
    /* Optional fields with configurations */
    bool has_wifi_config;
    WiFiConfig wifi_config;
    bool has_soft_ap_config;
    SoftAPConfig soft_ap_config;
    bool has_mqtt_config;
    MQTTConfig mqtt_config;
    /* Optional flags to enable/disable features */
    bool has_enable_custom_mdns;
    bool enable_custom_mdns;
    bool has_enable_custom_dhcp;
    bool enable_custom_dhcp;
    bool has_enable_custom_ntp;
    bool enable_custom_ntp;
    bool has_enable_mqtt;
    bool enable_mqtt;
    bool has_enable_esp_now;
    bool enable_esp_now;
    bool has_enable_websocket;
    bool enable_websocket;
    bool has_enable_api_server;
    bool enable_api_server;
} Settings;


#ifdef __cplusplus
extern "C" {
#endif

/* Helper constants for enums */
#define _BootSequence_MIN BootSequence_BOOT_SEQUENCE_UNKNWON
#define _BootSequence_MAX BootSequence_BOOT_SEQUENCE_STATION_FALLBACK_AP
#define _BootSequence_ARRAYSIZE ((BootSequence)(BootSequence_BOOT_SEQUENCE_STATION_FALLBACK_AP+1))












#define Settings_boot_sequence_ENUMTYPE BootSequence


/* Initializer values for message structs */
#define MQTTTopics_init_default                  {{{NULL}, NULL}}
#define MQTTKeepAlive_init_default               {0}
#define MQTTCleanSession_init_default            {0}
#define MQTTConfig_init_default                  {{{NULL}, NULL}, false, 0, false, MQTTTopics_init_default, false, MQTTTopics_init_default, false, MQTTKeepAlive_init_default, false, MQTTCleanSession_init_default, false, 0}
#define SoftAPConfig_init_default                {{{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}, false, 0, false, 0, false, 0, false, DHCPConfig_init_default, false, MDNSConfig_init_default}
#define MDNSConfig_init_default                  {{{NULL}, NULL}, {{NULL}, NULL}, false, 0, false, 0}
#define DNSConfig_init_default                   {{{NULL}, NULL}, {{NULL}, NULL}}
#define DHCPConfig_init_default                  {{{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}}
#define NTPConfig_init_default                   {{{NULL}, NULL}, 0}
#define APConfig_init_default                    {{{NULL}, NULL}, {{NULL}, NULL}, false, 0, false, 0, false, 0, false, DNSConfig_init_default, false, DHCPConfig_init_default, false, MDNSConfig_init_default}
#define WiFiConfig_init_default                  {{{NULL}, NULL}}
#define Settings_init_default                    {{{NULL}, NULL}, _BootSequence_MIN, false, WiFiConfig_init_default, false, SoftAPConfig_init_default, false, MQTTConfig_init_default, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0}
#define MQTTTopics_init_zero                     {{{NULL}, NULL}}
#define MQTTKeepAlive_init_zero                  {0}
#define MQTTCleanSession_init_zero               {0}
#define MQTTConfig_init_zero                     {{{NULL}, NULL}, false, 0, false, MQTTTopics_init_zero, false, MQTTTopics_init_zero, false, MQTTKeepAlive_init_zero, false, MQTTCleanSession_init_zero, false, 0}
#define SoftAPConfig_init_zero                   {{{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}, false, 0, false, 0, false, 0, false, DHCPConfig_init_zero, false, MDNSConfig_init_zero}
#define MDNSConfig_init_zero                     {{{NULL}, NULL}, {{NULL}, NULL}, false, 0, false, 0}
#define DNSConfig_init_zero                      {{{NULL}, NULL}, {{NULL}, NULL}}
#define DHCPConfig_init_zero                     {{{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}}
#define NTPConfig_init_zero                      {{{NULL}, NULL}, 0}
#define APConfig_init_zero                       {{{NULL}, NULL}, {{NULL}, NULL}, false, 0, false, 0, false, 0, false, DNSConfig_init_zero, false, DHCPConfig_init_zero, false, MDNSConfig_init_zero}
#define WiFiConfig_init_zero                     {{{NULL}, NULL}}
#define Settings_init_zero                       {{{NULL}, NULL}, _BootSequence_MIN, false, WiFiConfig_init_zero, false, SoftAPConfig_init_zero, false, MQTTConfig_init_zero, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0}

/* Field tags (for use in manual encoding/decoding) */
#define MQTTTopics_topic_tag                     1
#define MQTTKeepAlive_keep_alive_tag             1
#define MQTTCleanSession_cleanSession_tag        1
#define MQTTConfig_host_tag                      1
#define MQTTConfig_port_tag                      2
#define MQTTConfig_publish_topics_tag            3
#define MQTTConfig_subscribe_topics_tag          4
#define MQTTConfig_keep_alive_tag                5
#define MQTTConfig_clean_session_tag             6
#define MQTTConfig_timeout_tag                   7
#define MDNSConfig_device_name_tag               1
#define MDNSConfig_service_type_tag              2
#define MDNSConfig_port_tag                      3
#define MDNSConfig_advertise_ip_tag              4
#define DNSConfig_primary_dns_tag                1
#define DNSConfig_secondary_dns_tag              2
#define DHCPConfig_ip_tag                        1
#define DHCPConfig_gateway_tag                   2
#define DHCPConfig_subnet_tag                    3
#define SoftAPConfig_SSID_tag                    1
#define SoftAPConfig_password_tag                2
#define SoftAPConfig_psk_tag                     3
#define SoftAPConfig_channel_tag                 4
#define SoftAPConfig_hidden_tag                  5
#define SoftAPConfig_max_connection_tag          6
#define SoftAPConfig_dhcp_config_tag             7
#define SoftAPConfig_mdns_config_tag             8
#define NTPConfig_pool_server_name_tag           1
#define NTPConfig_port_tag                       2
#define APConfig_SSID_tag                        1
#define APConfig_password_tag                    2
#define APConfig_channel_tag                     3
#define APConfig_hidden_tag                      4
#define APConfig_max_connection_tag              5
#define APConfig_dns_config_tag                  6
#define APConfig_dhcp_config_tag                 7
#define APConfig_mdns_config_tag                 8
#define WiFiConfig_ap_config_tag                 1
#define Settings_device_name_tag                 1
#define Settings_boot_sequence_tag               2
#define Settings_wifi_config_tag                 3
#define Settings_soft_ap_config_tag              4
#define Settings_mqtt_config_tag                 5
#define Settings_enable_custom_mdns_tag          6
#define Settings_enable_custom_dhcp_tag          7
#define Settings_enable_custom_ntp_tag           8
#define Settings_enable_mqtt_tag                 9
#define Settings_enable_esp_now_tag              10
#define Settings_enable_websocket_tag            11
#define Settings_enable_api_server_tag           12

/* Struct field encoding specification for nanopb */
#define MQTTTopics_FIELDLIST(X, a) \
X(a, CALLBACK, REPEATED, STRING,   topic,             1)
#define MQTTTopics_CALLBACK pb_default_field_callback
#define MQTTTopics_DEFAULT NULL

#define MQTTKeepAlive_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    keep_alive,        1)
#define MQTTKeepAlive_CALLBACK NULL
#define MQTTKeepAlive_DEFAULT NULL

#define MQTTCleanSession_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, BOOL,     cleanSession,      1)
#define MQTTCleanSession_CALLBACK NULL
#define MQTTCleanSession_DEFAULT NULL

#define MQTTConfig_FIELDLIST(X, a) \
X(a, CALLBACK, SINGULAR, STRING,   host,              1) \
X(a, STATIC,   OPTIONAL, INT32,    port,              2) \
X(a, STATIC,   OPTIONAL, MESSAGE,  publish_topics,    3) \
X(a, STATIC,   OPTIONAL, MESSAGE,  subscribe_topics,   4) \
X(a, STATIC,   OPTIONAL, MESSAGE,  keep_alive,        5) \
X(a, STATIC,   OPTIONAL, MESSAGE,  clean_session,     6) \
X(a, STATIC,   OPTIONAL, INT32,    timeout,           7)
#define MQTTConfig_CALLBACK pb_default_field_callback
#define MQTTConfig_DEFAULT NULL
#define MQTTConfig_publish_topics_MSGTYPE MQTTTopics
#define MQTTConfig_subscribe_topics_MSGTYPE MQTTTopics
#define MQTTConfig_keep_alive_MSGTYPE MQTTKeepAlive
#define MQTTConfig_clean_session_MSGTYPE MQTTCleanSession

#define SoftAPConfig_FIELDLIST(X, a) \
X(a, CALLBACK, SINGULAR, STRING,   SSID,              1) \
X(a, CALLBACK, OPTIONAL, STRING,   password,          2) \
X(a, CALLBACK, OPTIONAL, STRING,   psk,               3) \
X(a, STATIC,   OPTIONAL, INT32,    channel,           4) \
X(a, STATIC,   OPTIONAL, BOOL,     hidden,            5) \
X(a, STATIC,   OPTIONAL, INT32,    max_connection,    6) \
X(a, STATIC,   OPTIONAL, MESSAGE,  dhcp_config,       7) \
X(a, STATIC,   OPTIONAL, MESSAGE,  mdns_config,       8)
#define SoftAPConfig_CALLBACK pb_default_field_callback
#define SoftAPConfig_DEFAULT NULL
#define SoftAPConfig_dhcp_config_MSGTYPE DHCPConfig
#define SoftAPConfig_mdns_config_MSGTYPE MDNSConfig

#define MDNSConfig_FIELDLIST(X, a) \
X(a, CALLBACK, SINGULAR, STRING,   device_name,       1) \
X(a, CALLBACK, SINGULAR, STRING,   service_type,      2) \
X(a, STATIC,   OPTIONAL, INT32,    port,              3) \
X(a, STATIC,   OPTIONAL, BOOL,     advertise_ip,      4)
#define MDNSConfig_CALLBACK pb_default_field_callback
#define MDNSConfig_DEFAULT NULL

#define DNSConfig_FIELDLIST(X, a) \
X(a, CALLBACK, OPTIONAL, STRING,   primary_dns,       1) \
X(a, CALLBACK, OPTIONAL, STRING,   secondary_dns,     2)
#define DNSConfig_CALLBACK pb_default_field_callback
#define DNSConfig_DEFAULT NULL

#define DHCPConfig_FIELDLIST(X, a) \
X(a, CALLBACK, OPTIONAL, STRING,   ip,                1) \
X(a, CALLBACK, OPTIONAL, STRING,   gateway,           2) \
X(a, CALLBACK, OPTIONAL, STRING,   subnet,            3)
#define DHCPConfig_CALLBACK pb_default_field_callback
#define DHCPConfig_DEFAULT NULL

#define NTPConfig_FIELDLIST(X, a) \
X(a, CALLBACK, SINGULAR, STRING,   pool_server_name,   1) \
X(a, STATIC,   SINGULAR, INT32,    port,              2)
#define NTPConfig_CALLBACK pb_default_field_callback
#define NTPConfig_DEFAULT NULL

#define APConfig_FIELDLIST(X, a) \
X(a, CALLBACK, SINGULAR, STRING,   SSID,              1) \
X(a, CALLBACK, SINGULAR, STRING,   password,          2) \
X(a, STATIC,   OPTIONAL, INT32,    channel,           3) \
X(a, STATIC,   OPTIONAL, BOOL,     hidden,            4) \
X(a, STATIC,   OPTIONAL, INT32,    max_connection,    5) \
X(a, STATIC,   OPTIONAL, MESSAGE,  dns_config,        6) \
X(a, STATIC,   OPTIONAL, MESSAGE,  dhcp_config,       7) \
X(a, STATIC,   OPTIONAL, MESSAGE,  mdns_config,       8)
#define APConfig_CALLBACK pb_default_field_callback
#define APConfig_DEFAULT NULL
#define APConfig_dns_config_MSGTYPE DNSConfig
#define APConfig_dhcp_config_MSGTYPE DHCPConfig
#define APConfig_mdns_config_MSGTYPE MDNSConfig

#define WiFiConfig_FIELDLIST(X, a) \
X(a, CALLBACK, REPEATED, MESSAGE,  ap_config,         1)
#define WiFiConfig_CALLBACK pb_default_field_callback
#define WiFiConfig_DEFAULT NULL
#define WiFiConfig_ap_config_MSGTYPE APConfig

#define Settings_FIELDLIST(X, a) \
X(a, CALLBACK, SINGULAR, STRING,   device_name,       1) \
X(a, STATIC,   SINGULAR, UENUM,    boot_sequence,     2) \
X(a, STATIC,   OPTIONAL, MESSAGE,  wifi_config,       3) \
X(a, STATIC,   OPTIONAL, MESSAGE,  soft_ap_config,    4) \
X(a, STATIC,   OPTIONAL, MESSAGE,  mqtt_config,       5) \
X(a, STATIC,   OPTIONAL, BOOL,     enable_custom_mdns,   6) \
X(a, STATIC,   OPTIONAL, BOOL,     enable_custom_dhcp,   7) \
X(a, STATIC,   OPTIONAL, BOOL,     enable_custom_ntp,   8) \
X(a, STATIC,   OPTIONAL, BOOL,     enable_mqtt,       9) \
X(a, STATIC,   OPTIONAL, BOOL,     enable_esp_now,   10) \
X(a, STATIC,   OPTIONAL, BOOL,     enable_websocket,  11) \
X(a, STATIC,   OPTIONAL, BOOL,     enable_api_server,  12)
#define Settings_CALLBACK pb_default_field_callback
#define Settings_DEFAULT NULL
#define Settings_wifi_config_MSGTYPE WiFiConfig
#define Settings_soft_ap_config_MSGTYPE SoftAPConfig
#define Settings_mqtt_config_MSGTYPE MQTTConfig

extern const pb_msgdesc_t MQTTTopics_msg;
extern const pb_msgdesc_t MQTTKeepAlive_msg;
extern const pb_msgdesc_t MQTTCleanSession_msg;
extern const pb_msgdesc_t MQTTConfig_msg;
extern const pb_msgdesc_t SoftAPConfig_msg;
extern const pb_msgdesc_t MDNSConfig_msg;
extern const pb_msgdesc_t DNSConfig_msg;
extern const pb_msgdesc_t DHCPConfig_msg;
extern const pb_msgdesc_t NTPConfig_msg;
extern const pb_msgdesc_t APConfig_msg;
extern const pb_msgdesc_t WiFiConfig_msg;
extern const pb_msgdesc_t Settings_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define MQTTTopics_fields &MQTTTopics_msg
#define MQTTKeepAlive_fields &MQTTKeepAlive_msg
#define MQTTCleanSession_fields &MQTTCleanSession_msg
#define MQTTConfig_fields &MQTTConfig_msg
#define SoftAPConfig_fields &SoftAPConfig_msg
#define MDNSConfig_fields &MDNSConfig_msg
#define DNSConfig_fields &DNSConfig_msg
#define DHCPConfig_fields &DHCPConfig_msg
#define NTPConfig_fields &NTPConfig_msg
#define APConfig_fields &APConfig_msg
#define WiFiConfig_fields &WiFiConfig_msg
#define Settings_fields &Settings_msg

/* Maximum encoded size of messages (where known) */
/* MQTTTopics_size depends on runtime parameters */
/* MQTTConfig_size depends on runtime parameters */
/* SoftAPConfig_size depends on runtime parameters */
/* MDNSConfig_size depends on runtime parameters */
/* DNSConfig_size depends on runtime parameters */
/* DHCPConfig_size depends on runtime parameters */
/* NTPConfig_size depends on runtime parameters */
/* APConfig_size depends on runtime parameters */
/* WiFiConfig_size depends on runtime parameters */
/* Settings_size depends on runtime parameters */
#define MQTTCleanSession_size                    2
#define MQTTKeepAlive_size                       11

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
