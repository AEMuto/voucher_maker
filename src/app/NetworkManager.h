#pragma once

#include <globals.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "Model.h"

class NetworkManager : public Observer {
 private:
  Model& model;
  bool isAPActive = false;
  bool isClientConnected = false;
  unsigned long lastConnectionAttempt = 0;
  const unsigned long CONNECTION_RETRY_INTERVAL = 5000; 

 public:
  NetworkManager(Model& model);
  ~NetworkManager() override;
  void onModelChanged(const ModelEventData& event) override;
  void begin();
  void update();
  void setupAP();

 private:
  void handleNetworkStateChange(NetworkState newState);

};