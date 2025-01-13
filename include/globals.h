#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <Preferences.h>
#include <nvs_flash.h>

// Constants for AP mode
const String AP_SSID = "Voucher_Maker-Configuration";
const String AP_PASSWORD = String((uint32_t)random(100000000, 999999999));
const int AP_CHANNEL = 1;
const int MAX_CLIENTS = 1;

enum class NetworkState {
    DISCONNECTED,           // No WiFi connection
    CONNECTING,             // Attempting to connect to main network
    CONNECTED,              // Connected to main network
    CONNECTION_FAILED,      // Connection attempt failed
    AP_MODE,                // Device is in Access Point mode
    WAITING_FOR_CLIENT,     // AP is active, waiting for user to connect
    CLIENT_CONNECTED,       // User has connected to our AP
    AWAITING_CREDENTIALS    // Waiting for user to submit credentials
};

enum class ApplicationState {
    BOOT,                   // Initial boot state
    STANDARD,               // Regular operation mode
    CONFIGURATION,          // Configuration mode (could be initial setup or later reconfiguration)
    SEARCH_MQTT_SERVER,     // Searching for MQTT broker
    CONNECT_MQTT_SERVER,    // Connecting to MQTT broker
    GENERATE_VOUCHER,       // Generating voucher
    VOUCHER_GENERATED,      // Voucher generated
    SEND_VOUCHER,           // Sending voucher to MQTT broker
    ERROR                   // Error state
};
