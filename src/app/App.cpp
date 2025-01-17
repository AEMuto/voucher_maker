#include "App.h"

App::App() {
  model = make_unique<Model>();
  uiManager = make_unique<UIManager>(*model);
  networkManager = make_unique<NetworkManager>(*model);
  Serial.println("Creating button manager");
  delay(100);
  buttonManager = make_unique<ButtonManager>([this](ButtonAction action) {
    Serial.println("Initiating button action");
    delay(100);
    handleButtonAction(action);
  });
  memoryMonitor = createMemoryMonitor();
}

void App::begin() {
  Serial.begin(115200);

  // Initialize hardware
  pinMode(PIN_POWER, OUTPUT);
  digitalWrite(PIN_POWER, HIGH);  // Enable screen backlight

  // Button setup
  pinMode(IO00_BTN, INPUT_PULLUP);
  pinMode(IO14_BTN, INPUT_PULLUP);
  pinMode(IO01_BTN, INPUT_PULLUP);

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
  buttonManager->begin();

  // Initialize memory monitor if available (DEBUG build)
  if (memoryMonitor) memoryMonitor->begin();
}

void App::loop() {
  // Update managers
  buttonManager->update();
  uiManager->update();
  networkManager->update();

  // Handle app state
  handleAppState();
}

void App::handleButtonAction(ButtonAction action) {
  // Handle reboot immediately if requested
  if (action == ButtonAction::REBOOT) {
    // Could add a quick UI feedback if needed
    Serial.println("Rebooting...");
    delay(100);  // Brief delay to ensure any final operations complete
    ESP.restart();
    return;  // Not strictly needed due to restart, but good practice
  }
  model->notifyObservers(ModelEventData::createButtonEvent(action));
}

void App::handleAppState() {
  // Main state machine logic
  switch (model->getAppState()) {
    case AppState::BOOT:
      // Serial.println("Boot state");
      // delay(1000);
      model->setAppState(AppState::NETWORK_SETUP);
      break;
    case AppState::NETWORK_SETUP:
      // Serial.println("Network setup state");
      // delay(1000);
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