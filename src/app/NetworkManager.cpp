#include "NetworkManager.h"

NetworkManager::NetworkManager(Model& model) : model(model) {
  model.addObserver(this);
}

NetworkManager::~NetworkManager() {
  model.removeObserver(this);
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

void NetworkManager::begin() { // main setup
  // get wan credentials from model?
}

void NetworkManager::update() { // main loop, Needed?
  // handle network state changes?
}

void NetworkManager::handleNetworkStateChange(NetworkState newState) {
  // React to network state changes
  switch (newState) {
    case NetworkState::DISCONNECTED:
      // If model has wan credentials, attempt to connect else set up AP, web server etc.
      break;
    case NetworkState::AP_MODE:
      // Set up AP if not already done
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
      break;
    case NetworkState::CONNECTION_FAILED:
      // Connection attempt failed
      break;
    case NetworkState::CONNECTED:
      // Transition to main network
      break;
    default:
      break;
  }
}