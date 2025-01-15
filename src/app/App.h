#pragma once

using namespace std; 

#include <Arduino.h>
#include <memory>

#include <pins.h>
#include "Model.h"
#include "NetworkManager.h"
#include "UIManager.h"

#ifdef DEBUG
#include "MemoryMonitor.h"
#endif

class App {
 private:
  unique_ptr<Model> model;
  unique_ptr<UIManager> uiManager;
  unique_ptr<NetworkManager> networkManager;
#ifdef DEBUG
  unique_ptr<MemoryMonitor> memoryMonitor;
#endif

 public:
  App();
  void begin();
  void loop();

 private:
  void handleBootState();
  void handleNetworkSetup();
};