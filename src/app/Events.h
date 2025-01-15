enum class ModelEvent {
    NETWORK_STATUS_CHANGED,
    APP_STATE_CHANGED,
    VOUCHER_GENERATED,
    CREDENTIALS_UPDATED,
    ERROR_OCCURRED
};

struct ModelEventData {
    ModelEvent event;
    void* data;  // Can hold different types of data depending on the event
};