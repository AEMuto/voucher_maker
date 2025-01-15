#pragma once

#include <globals.h>

#include "Model.h"
#include "NetworkManager.h"
#include "UIManager.h"
#include "MemoryMonitor.h"

class App {
 private:
  unique_ptr<Model> model;
  unique_ptr<UIManager> uiManager;
  unique_ptr<NetworkManager> networkManager;
  unique_ptr<MemoryMonitorBase> memoryMonitor;

 public:
  App();
  void begin();
  void loop();

 private:
  void handleAppState();
  void handleBootState();
  void handleNetworkSetup();
};