#include "WifiConfig.h"

#define NVS_KEY "wifiKey123" // Encryption key for NVS

void WifiConfig::begin() {
    apSSID = "ESP32-" + String((uint32_t)ESP.getEfuseMac(), HEX);
    apPassword = String((uint32_t)random(100000000, 999999999));
    
    startAP();
    displayQR();
    setupWebServer();
}

void WifiConfig::startAP() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(apSSID.c_str(), apPassword.c_str());
}

void WifiConfig::displayQR() {
    String configUrl = "http://" + WiFi.softAPIP().toString() + "/config";
    String qrData = "WIFI:T:WPA;S:" + apSSID + ";P:" + apPassword + ";URL:" + configUrl + ";;";

    // Create instruction label
    instructionLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(instructionLabel, "Scan QR code to configure WiFi");
    lv_obj_set_style_text_color(instructionLabel, lv_color_white(), 0);
    lv_obj_align(instructionLabel, LV_ALIGN_TOP_MID, 0, 10);

    // Create QR code
    qrcode = lv_qrcode_create(lv_scr_act(), 150, lv_color_black(), lv_color_white());
    lv_qrcode_update(qrcode, qrData.c_str(), qrData.length());
    lv_obj_center(qrcode);

    // Create network info label
    networkLabel = lv_label_create(lv_scr_act());
    lv_label_set_text_fmt(networkLabel, "Network: %s\nPassword: %s", apSSID.c_str(), apPassword.c_str());
    lv_obj_set_style_text_color(networkLabel, lv_color_white(), 0);
    lv_obj_align(networkLabel, LV_ALIGN_BOTTOM_MID, 0, -10);
}

void WifiConfig::setupWebServer() {
    server.on("/config", HTTP_GET, [this]() {
        String html = "<form method='POST' action='/save'>"
                     "<input type='text' name='ssid' placeholder='WiFi SSID'><br>"
                     "<input type='password' name='password' placeholder='WiFi Password'><br>"
                     "<input type='submit' value='Save'></form>";
        server.send(200, "text/html", html);
    });

    server.on("/save", HTTP_POST, [this]() {
        String ssid = server.arg("ssid");
        String password = server.arg("password");
        
        Preferences prefs;
        prefs.begin("wifi", false);
        prefs.putString("ssid", ssid);
        prefs.putString("password", password);
        prefs.end();
        
        server.send(200, "text/plain", "Saved. ESP32 will now connect to your network.");
        
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), password.c_str());
    });
    
    server.begin();
}

void WifiConfig::handle() {
    server.handleClient();
}

bool WifiConfig::isConfigured() {
    Preferences prefs;
    prefs.begin("wifi", true, NVS_KEY);
    bool hasConfig = prefs.isKey("ssid");
    prefs.end();
    return hasConfig;
}