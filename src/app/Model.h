#pragma once

#ifndef NVS_KEY
#define NVS_KEY "wifi1234"
#endif

#include <Preferences.h>
#include <globals.h>

#include "../utils/PasswordGenerator.h"
#include "Observer.h"

using namespace std;

class Model {
 private:
  vector<Observer*> observers;
  AppState currentAppState;
  NetworkState currentNetworkState;
  ErrorState currentErrorState;
  CredentialsData wanCredentials;
  CredentialsData apCredentials;
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
  static int getAPChannel();
  static int getMaxClients();

  // Credentials management
  void setWANCredentials(CredentialsData& credentials);
  void setAPCredentials(CredentialsData& credentials);
  const CredentialsData& getWANCredentials() const;
  const CredentialsData& getAPCredentials() const;

  // States management
  void setAppState(AppState newState);
  AppState getAppState() const;
  void setNetworkState(NetworkState newState);
  NetworkState getNetworkState() const;
  void setErrorState(ErrorState newState);
  ErrorState getErrorState() const;

 private:
  void retrieveWANCredentials();
  void writeWANCredentials();
};