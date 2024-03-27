#ifndef ESP_NOW_LISTENER_H
#define ESP_NOW_LISTENER_H

#include <Arduino.h>

// Function to handle different command codes
void handle_ping();
void handle_pong();
void handle_play_tone();
void handle_play_tone_ok();
void handle_play_tx();
void handle_play_tx_ok();
void handle_sync_ap();
void handle_sync_ap_ok();
void handle_reboot();

// Callback when data is received
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len);

// Function to initialize ESP-NOW
void initESP_Now();

#endif  // ESP_NOW_LISTENER_H
 
