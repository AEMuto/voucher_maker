#pragma once
#include <Preferences.h>
#include <WebServer.h>
#include <WiFi.h>
#include <lvgl.h>

#include "tdisplay_config.h"

/**
 * @brief Setup and manage network configuration (Wifi)
 *
 */
class NetworkSetup {
 private:
  WebServer server;
  String apSSID = "ESP32-" + String((uint32_t)ESP.getEfuseMac(), HEX);
  String apPassword = String((uint32_t)random(100000000, 999999999));
  int connectedDevices = 0;

  void start_local_AP();
  void launch_webserver();
  void display_local_wifi_QR();
  void get_auth_devices();
  void display_config_form_QR();
  std::tuple<String, String> get_wan_credentials();

 public:
  void start_config();
  void handle_webclient();
  bool is_configured();
  void connect_to_wan();
  void wifi_scan();
};