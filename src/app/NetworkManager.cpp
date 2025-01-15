#include "NetworkManager.h"

explicit NetworkManager::NetworkManager(Model& model) : model(model) {
  model.addObserver(this);
}

void NetworkManager::begin() {
  // Initialize WiFi
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.setSleep(false);
}

void NetworkManager::update() {
  // Check if we need to switch to AP mode
  if (WiFi.status() != WL_CONNECTED) {
    model.setNetworkState(NetworkState::AP_MODE);
  }
}

void NetworkManager::setupAccessPoint() {
  // Access configuration through Model's getters
  WiFi.softAP(
      model.getAccessPointSSID().c_str(),
      model.getAccessPointPWD().c_str(),
      model.getAccessPointChannel(),
      false,  // Hidden SSID
      model.getMaxClients());
}

void NetworkManager::onModelChanged(const ModelEventData& event) {
  switch (event.event) {
    case ModelEvent::NETWORK_STATUS_CHANGED:
      handleNetworkStateChange(*static_cast<NetworkState*>(event.data));
      break;
    // Handle other relevant events
    default:
      break;
  }
}

void NetworkManager::handleNetworkStateChange(NetworkState newState) {
  // React to network state changes
  switch (newState) {
    case NetworkState::AP_MODE:
      setupAccessPoint();
      break;
    // Handle other states
    default:
      break;
  }
}