#include "App.h"

App::App() {
  model = make_unique<Model>();
  uiManager = make_unique<UIManager>(*model);
  networkManager = make_unique<NetworkManager>(*model);
  memoryMonitor = createMemoryMonitor();
}

void App::begin() {
  Serial.begin(115200);

  // Initialize hardware
  pinMode(PIN_POWER, OUTPUT);
  digitalWrite(PIN_POWER, HIGH);  // Enable screen backlight

  // Initialize NVS
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);

  // Initialize managers
  uiManager->begin();
  networkManager->begin();

  // Initialize memory monitor if available (DEBUG build)
  if (memoryMonitor) memoryMonitor->begin();

  // Next State
  model->setAppState(AppState::NETWORK_SETUP);
}

void App::loop() {  
  // Update managers
  uiManager->update();
  networkManager->update();
  
  // Handle app state
  handleAppState();
}

void App::handleAppState() {
  // Main state machine logic
  switch (model->getAppState()) {
    case AppState::BOOT:
      handleBootState();
      break;
    case AppState::NETWORK_SETUP:
      handleNetworkSetup();
      break;
    case AppState::READY:
      // Ready state logic
      break;
    case AppState::GENERATING_VOUCHER:
      // Voucher generation logic
      break;
    case AppState::VIEWING_VOUCHERS:
      // Voucher viewing logic
      break;
    case AppState::PRINTING:
      // Printing logic
      break;
    case AppState::ERROR:
      // Error state logic
      break;
  }
}

void App::handleBootState() {
  // Boot state logic
  // Transition to next state
  model->setAppState(AppState::NETWORK_SETUP);
}

void App::handleNetworkSetup() {
  try {
    // Network setup logic
    // Transition to next state
    model->setAppState(AppState::READY);
  } catch (const exception& e) {
    // Handle network setup error
    model->setErrorState(ErrorState::NETWORK_UNREACHABLE);
  }
  
}