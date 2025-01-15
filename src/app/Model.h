#pragma once

#ifndef NVS_KEY
#define NVS_KEY "wifi1234"
#endif

#include <globals.h>
#include <Preferences.h>
#include <nvs_flash.h>

#include "../utils/PasswordGenerator.h"
#include "Observer.h"

using namespace std;

class Model {
 private:
  vector<Observer*> observers;
  AppState currentAppState;
  NetworkState currentNetworkState;
  ErrorState currentErrorState;
  CredentialsData credentials;
  String accessPointSSID;
  String accessPointPWD;
  String nvsKey;

  // Configuration constants
  static constexpr const char* DEFAULT_AP_SSID = "Voucher_Maker-Configuration";
  static constexpr int AP_CHANNEL = 1;
  static constexpr int MAX_CLIENTS = 1;

 public:
  Model();
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