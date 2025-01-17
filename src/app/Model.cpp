#include "Model.h"

Model::Model() : currentAppState(AppState::BOOT),
                 currentNetworkState(NetworkState::DISCONNECTED),
                 currentErrorState(ErrorState::NONE),
                 apCredentials(CredentialsData{DEFAULT_AP_SSID, pwdGen(8)}),
                 nvsKey(NVS_KEY) {}

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

int Model::getAPChannel() { return AP_CHANNEL; }
int Model::getMaxClients() { return MAX_CLIENTS; }

void Model::setWANCredentials(CredentialsData& credentials) {
  wanCredentials.ssid = credentials.ssid;
  wanCredentials.password = credentials.password;
}

void Model::setAPCredentials(CredentialsData& credentials) {
  apCredentials.ssid = credentials.ssid;
  apCredentials.password = credentials.password;
}

const CredentialsData& Model::getWANCredentials() const { return wanCredentials; }
const CredentialsData& Model::getAPCredentials() const { return apCredentials; }

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

void Model::retrieveWANCredentials() {
  Preferences preferences;
  preferences.begin("wan", true);
  wanCredentials.ssid = preferences.getString("ssid", "");
  wanCredentials.password = preferences.getString("password", "");
  preferences.end();
  ModelEventData event{ModelEvent::CREDENTIALS_UPDATED, &wanCredentials};
  notifyObservers(event);
}

void Model::writeWANCredentials() {
  Preferences preferences;
  preferences.begin("wan", false);
  preferences.putString("ssid", wanCredentials.ssid);
  preferences.putString("password", wanCredentials.password);
  preferences.end();
}

