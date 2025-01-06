#pragma once
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <lvgl.h>

/**
 * @brief Class to handle WiFi configuration.
 * 
 */
class WifiConfig {
public:
    void begin();
    void handle();
    bool isConfigured();

private:
    void startAP();
    void displayQR();
    void setupWebServer();

    WebServer server;
    String apSSID;
    String apPassword;
    lv_obj_t *instructionLabel;
    lv_obj_t *networkLabel;
    lv_obj_t *qrcode;
};