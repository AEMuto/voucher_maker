#include "App.h"

App::App() {
  model = make_unique<Model>();
  uiManager = make_unique<UIManager>(*model);
  networkManager = make_unique<NetworkManager>(*model);
#ifdef DEBUG
  memoryMonitor = make_unique<MemoryMonitor>(*model);
#endif
}

void App::begin() {
  model->setAppState(AppState::BOOT);
  
  // Initialize hardware
  pinMode(PIN_POWER, OUTPUT);
  digitalWrite(PIN_POWER, HIGH);  // Enable screen backlight
  model->begin();
  uiManager->begin();
  networkManager->begin();
#ifdef DEBUG
  memoryMonitor->begin();
#endif
}

void App::loop() {  // Update managers
  uiManager->update();
  networkManager->update();
#ifdef DEBUG
  memoryMonitor->update();
#endif

  // Main state machine
  switch (model->getAppState()) {
    case AppState::BOOT:
      handleBootState();
      break;
    case AppState::NETWORK_SETUP:
      handleNetworkSetup();
      break;
      // ... other states
  }
}

void App::handleBootState() {
  // Boot state logic
  // Transition to next state
  model->setAppState(AppState::NETWORK_SETUP);
}

void App::handleNetworkSetup() {
  // Network setup state logic
  // Transition to next state
  model->setAppState(AppState::READY);
}