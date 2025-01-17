#pragma once

#include "data_types.h"
#include "states.h"

enum class ButtonAction {
  SELECT,
  BACK,
  REBOOT,
  UP,
  DOWN,
  NONE
};

enum class ModelEvent {
  NETWORK_STATUS_CHANGED,
  APP_STATE_CHANGED,
  VOUCHER_GENERATED,
  CREDENTIALS_UPDATED,
  PRINTER_STATUS_CHANGED,
  ERROR_OCCURRED,
  BUTTON_PRESSED
};

// The event wrapper struct
struct ModelEventData {
  ModelEvent event;
  void* data;

  // Will look like {APP_STATE_CHANGED, AppState::BOOT}
  static ModelEventData createNetworkStateEvent(NetworkState* state) {
    return ModelEventData{ModelEvent::NETWORK_STATUS_CHANGED, static_cast<void*>(state)};
  }

  static ModelEventData createVoucherEvent(VoucherData* voucher) {
    return ModelEventData{ModelEvent::VOUCHER_GENERATED, static_cast<void*>(voucher)};
  }

  static ModelEventData createCredentialsEvent(CredentialsData* creds) {
    return ModelEventData{ModelEvent::CREDENTIALS_UPDATED, static_cast<void*>(creds)};
  }

  static ModelEventData createErrorEvent(ErrorData* error) {
    return ModelEventData{ModelEvent::ERROR_OCCURRED, static_cast<void*>(error)};
  }

  static ModelEventData createPrinterStatusEvent(PrinterStatus* status) {
    return ModelEventData{ModelEvent::PRINTER_STATUS_CHANGED, static_cast<void*>(status)};
  }

  // Will look like {BUTTON_PRESSED, SELECT}
  static ModelEventData createButtonEvent(ButtonAction action) {
    auto* data = new ButtonAction(action);
    return ModelEventData{ModelEvent::BUTTON_PRESSED, static_cast<void*>(data)};
  }
};