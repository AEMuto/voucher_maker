#pragma once

#include "states.h"
#include "data_types.h"

enum class ModelEvent {
    NETWORK_STATUS_CHANGED,
    APP_STATE_CHANGED,
    VOUCHER_GENERATED,
    CREDENTIALS_UPDATED,
    PRINTER_STATUS_CHANGED,
    ERROR_OCCURRED
};

// The event wrapper struct
struct ModelEventData {
    ModelEvent event;
    void* data;

    // Helper methods to safely create event data
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
};