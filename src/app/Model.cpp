#include "Model.h"

Model::Model() : currentAppState(AppState::BOOT),
                 currentNetworkState(NetworkState::DISCONNECTED),
                 currentErrorState(ErrorState::NONE),
                 accessPointSSID(DEFAULT_AP_SSID),
                 nvsKey(NVS_KEY) {
  accessPointPWD = pwdGen(8);
}

void Model::addObserver(Observer* observer) {
  observers.push_back(observer);
}

void Model::removeObserver(Observer* observer) {
  observers.erase(
      remove(observers.begin(), observers.end(), observer),
      observers.end());
}

void Model::notifyObservers(const ModelEventData& event) {
  for (auto observer : observers) {
    observer->onModelChanged(event);
  }
}

const String& Model::getAccessPointSSID() const { return accessPointSSID; }
const String& Model::getAccessPointPWD() const { return accessPointPWD; }
int Model::getAccessPointChannel() { return AP_CHANNEL; }
int Model::getMaxClients() { return MAX_CLIENTS; }

void Model::setAppState(AppState newState) {
  if (currentAppState != newState) {
    currentAppState = newState;
    ModelEventData event{ModelEvent::APP_STATE_CHANGED, &currentAppState};
    notifyObservers(event);
  }
}

AppState Model::getAppState() const { return currentAppState; }

void Model::setNetworkState(NetworkState newState) {
  if (currentNetworkState != newState) {
    currentNetworkState = newState;
    ModelEventData event{ModelEvent::NETWORK_STATUS_CHANGED, &currentNetworkState};
    notifyObservers(event);
  }
}

NetworkState Model::getNetworkState() const { return currentNetworkState; }

void Model::setErrorState(ErrorState newState) {
  if (currentErrorState != newState) {
    currentErrorState = newState;
    ModelEventData event{ModelEvent::ERROR_OCCURRED, &currentErrorState};
    notifyObservers(event);
  }
}

ErrorState Model::getErrorState() const { return currentErrorState; }