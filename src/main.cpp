#include <globals.h>
#include <pins.h>

#include "app/App.h"
#include "app/Model.h"
#include "app/NetworkManager.h"
#include "app/UIManager.h"

void setup() {
  /* (POWER ON)IO15 must be set to HIGH before starting,
  /* otherwise the screen will not display when using battery */
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);

  Serial.begin(115200);

  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
  
  // Intialize App
  Model model;
  UIManager uiManager;
  NetworkManager networkManager;
  App app(model, uiManager, networkManager);
  app.begin();
}