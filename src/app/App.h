#pragma once

#include <globals.h>
#include <nvs_flash.h>

#include "Model.h"
#include "NetworkManager.h"
#include "UIManager.h"
#include "MemoryMonitor.h"
#include "ButtonManager.h"

class App {
 private:
  unique_ptr<Model> model;
  unique_ptr<UIManager> uiManager;
  unique_ptr<NetworkManager> networkManager;
  unique_ptr<MemoryMonitorBase> memoryMonitor;
  unique_ptr<ButtonManager> buttonManager;

 public:
  App();
  void begin();
  void loop();

 private:
  void handleAppState();
  void handleButtonAction(ButtonAction action);
};