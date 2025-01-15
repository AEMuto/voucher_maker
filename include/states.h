#pragma once

enum struct NetworkState {
  DISCONNECTED,          // No WiFi connection
  AP_MODE,               // Device is in Access Point mode
  WAITING_FOR_CLIENT,    // AP is active, waiting for user to connect
  CLIENT_CONNECTED,      // User has connected to our AP
  AWAITING_CREDENTIALS,  // Waiting for user to submit credentials
  CONNECTING,            // Attempting to connect to main network
  CONNECTION_FAILED,     // Connection attempt failed
  CONNECTED,             // Connected to main network
};

enum struct AppState {
  BOOT,
  HARDWARE_CHECK,
  NETWORK_SETUP,
  READY,
  GENERATING_VOUCHER,
  VIEWING_VOUCHERS,
  PRINTING,
  ERROR
};

enum struct ErrorState {
  NONE,
  NETWORK_UNREACHABLE,
  SERVER_UNREACHABLE,
  INVALID_CREDENTIALS,
  MQTT_ERROR,
  PRINTER_ERROR,
  CRITICAL_ERROR
};