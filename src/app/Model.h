#pragma once

#include <vector>

#include "../utils/PasswordGenerator.h"
#include "Observer.h"

using namespace std;

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

struct WiFiCredentials {
  String ssid;
  String password;
};

class Model {
 private:
  vector<Observer*> observers;
  AppState currentAppState;
  NetworkState currentNetworkState;
  ErrorState currentErrorState;
  String accessPointSSID;
  String accessPointPWD;

  // Configuration constants
  static constexpr const char* DEFAULT_AP_SSID = "Voucher_Maker-Configuration";
  static constexpr int AP_CHANNEL = 1;
  static constexpr int MAX_CLIENTS = 1;

 public:
  Model();
  void begin();
  // Observer management
  void addObserver(Observer* observer);
  void removeObserver(Observer* observer);
  void notifyObservers(const ModelEventData& event);

  // Getters for network configuration
  const String& getAccessPointSSID() const;
  const String& getAccessPointPWD() const;
  static int getAccessPointChannel();
  static int getMaxClients();

  // States management
  void setAppState(AppState newState);
  AppState getAppState() const;
  void setNetworkState(NetworkState newState);
  NetworkState getNetworkState() const;
  void setErrorState(ErrorState newState);
  ErrorState getErrorState() const;
};