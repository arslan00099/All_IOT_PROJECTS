#include "esp_now_listener.h"
#include "esp_now_packet.pb.h"  // Assuming you have the generated nanopb header file included

// Function to handle different command codes
void handle_ping() {
    // Handle PING command
}

void handle_pong() {
    // Handle PONG command
}

void handle_play_tone() {
    // Handle PLAY_TONE command
}

void handle_play_tone_ok() {
    // Handle PLAY_TONE_OK command
}

void handle_play_tx() {
    // Handle PLAY_TX command
}

void handle_play_tx_ok() {
    // Handle PLAY_TX_OK command
}

void handle_sync_ap() {
    // Handle SYNC_AP command
}

void handle_sync_ap_ok() {
    // Handle SYNC_AP_OK command
}

void handle_reboot() {
    // Handle REBOOT command
}

// Callback when data is received
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
    // Parse the received data using nanopb
    esp_now_msg incoming_msg;
    pb_istream_t istream = pb_istream_from_buffer(incomingData, len);
    bool status = pb_decode(&istream, esp_now_msg_fields, &incoming_msg);

    // Compare the target_SSID with the device's own SSID
    if (status && strcmp(incoming_msg.target_SSID, your_device_SSID) == 0) {
        // Handle different command codes using a switch statement
        switch (incoming_msg.command_code) {
            case CommandCode_PING:
                handle_ping();
                break;
            case CommandCode_PONG:
                handle_pong();
                break;
            case CommandCode_PLAY_TONE:
                handle_play_tone();
                break;
            case CommandCode_PLAY_TONE_OK:
                handle_play_tone_ok();
                break;
            case CommandCode_PLAY_TX:
                handle_play_tx();
                break;
            case CommandCode_PLAY_TX_OK:
                handle_play_tx_ok();
                break;
            case CommandCode_SYNC_AP:
                handle_sync_ap();
                break;
            case CommandCode_SYNC_AP_OK:
                handle_sync_ap_ok();
                break;
            case CommandCode_REBOOT:
                handle_reboot();
                break;
            default:
                // Handle unknown command code
                break;
        }
    }
}

// Function to initialize ESP-NOW
void initESP_Now() {
    if (esp_now_init() != 0) {
        debugln("Error initializing ESP-NOW");
        return;
    }
    // Set ESP-NOW Role
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    // Register peer
    esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
    // Register the callback function for data reception
    esp_now_register_recv_cb(OnDataRecv);
}
