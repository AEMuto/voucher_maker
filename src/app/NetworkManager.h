#pragma once

#include <globals.h>
#include <WiFi.h>

#include "Model.h"

class NetworkManager : public Observer {
 private:
  Model& model;

 public:
  NetworkManager(Model& model);
  ~NetworkManager() override;
  void begin();
  void update();
  void setupAccessPoint();
  void onModelChanged(const ModelEventData& event) override;

 private:
  void handleNetworkStateChange(NetworkState newState);

};