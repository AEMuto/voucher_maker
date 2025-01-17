#pragma once

#include <Arduino.h>
#include "states.h"

struct VoucherData {
    String code;
    uint32_t duration;  // in seconds
    time_t generatedAt;
    AppState generatedDuring;
    // Add other voucher-related fields
};

struct CredentialsData {
    String ssid;
    String password;
    // Add other credential-related fields
};

struct ErrorData {
    String message;
    uint32_t code;
    ErrorState errorState;
    // Add other error-related fields
};

struct PrinterStatus {
    bool isConnected;
    bool isReady;
    String errorMessage;
    ErrorState printerError;
    // Add other printer status fields
};