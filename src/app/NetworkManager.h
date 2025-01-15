#pragma once
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <nvs_flash.h>

#include "Model.h"

class NetworkManager : public Observer {
 private:
  Model& model;

 public:
  explicit NetworkManager(Model& model);
  void begin();
  void update();
  void setupAccessPoint();
  void onModelChanged(const ModelEventData& event) override;

 private:
  void handleNetworkStateChange(NetworkState newState);

};