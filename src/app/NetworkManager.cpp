#include "NetworkManager.h"

NetworkManager::NetworkManager(Model& model) : model(model) {
  model.addObserver(this);
}

NetworkManager::~NetworkManager() {
  model.removeObserver(this);
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
  if (WiFi.status() != WL_CONNECTED &&
      model.getNetworkState() != NetworkState::DISCONNECTED) {
    model.setNetworkState(NetworkState::DISCONNECTED);
  }
}

void NetworkManager::setupAccessPoint() {
  // Access configuration through Model's getters
  WiFi.softAP(
      model.getAccessPointSSID().c_str(),
      model.getAccessPointPWD().c_str(),
      model.getAccessPointChannel(),
      0,
      model.getMaxClients());
}

void NetworkManager::onModelChanged(const ModelEventData& event) {
  switch (event.event) {
    case ModelEvent::NETWORK_STATUS_CHANGED:
      handleNetworkStateChange(*static_cast<NetworkState*>(event.data));
      break;
    case ModelEvent::CREDENTIALS_UPDATED:
      // Handle credentials update
      break;
    // Handle other relevant events
    default:
      break;
  }
}

void NetworkManager::handleNetworkStateChange(NetworkState newState) {
  // React to network state changes
  switch (newState) {
    case NetworkState::DISCONNECTED:
      // Transition to AP mode
      model.setNetworkState(NetworkState::AP_MODE);
      break;
    case NetworkState::AP_MODE:
      setupAccessPoint();
      break;
    case NetworkState::WAITING_FOR_CLIENT:
      // AP is active, waiting for user to connect
      break;
    case NetworkState::CLIENT_CONNECTED:
      // User has connected to our AP
      break;
    case NetworkState::AWAITING_CREDENTIALS:
      // Waiting for user to submit credentials
      break;
    case NetworkState::CONNECTING:
      // Attempt to connect to main network
      WiFi.begin();
      break;
    case NetworkState::CONNECTION_FAILED:
      // Connection attempt failed
      model.setNetworkState(NetworkState::DISCONNECTED);
      break;
    case NetworkState::CONNECTED:
      // Transition to main network
      model.setNetworkState(NetworkState::CONNECTED);
      break;
    default:
      break;
  }
}