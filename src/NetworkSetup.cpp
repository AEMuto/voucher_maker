#include "NetworkSetup.h"

#ifndef NVS_KEY
#define NVS_KEY "wifiKey123"  // Fallback key for NVS
#endif

void NetworkSetup::start_config() {
  start_local_AP();
  launch_webserver();
  display_local_wifi_QR();
}

void NetworkSetup::start_local_AP() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSSID.c_str(), apPassword.c_str());
}

void NetworkSetup::handle_webclient() {
  server.handleClient();
}

void NetworkSetup::get_auth_devices() {}

std::tuple<String, String> NetworkSetup::get_wan_credentials() {
  Preferences prefs;
  prefs.begin("wifi", true, NVS_KEY);
  String ssid = prefs.getString("ssid", "");
  String password = prefs.getString("pwd", "");
  prefs.end();
  return std::make_tuple(ssid, password);
}

bool NetworkSetup::is_configured() {
  auto [ssid, password] = this->get_wan_credentials();
  return ssid.length() > 0;
}

void NetworkSetup::connect_to_wan() {
  int tries = 0;
  Preferences prefs;
  prefs.begin("wifi", true, NVS_KEY);
  String ssid = prefs.getString("ssid", "");
  String password = prefs.getString("password", "");
  prefs.end();

  if (ssid.length() > 0) {
    while (WiFi.status() != WL_CONNECTED && tries < 3) {
      WiFi.begin(ssid.c_str(), password.c_str());
      delay(5000);
      tries++;
    }
    if (tries == 3) {
      // Failed to connect to the main WiFi network
      // TODO: It means we are outside the range of the registered wan AP
    }
  }
}

void NetworkSetup::wifi_scan() {
  // TODO: Should return a list of available networks
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; ++i) {
    Serial.print(WiFi.SSID(i));
    Serial.print(" ");
    Serial.println(WiFi.RSSI(i));
  }
}

void NetworkSetup::display_local_wifi_QR() {
  String qrData = "WIFI:T:WPA;S:" + apSSID + ";P:" + apPassword + ";;";
  String configURL = "http://" + WiFi.softAPIP().toString() + "/config";

  // Create Flex container
  lv_obj_t *container = lv_obj_create(lv_scr_act());
  lv_obj_set_size(container, lv_pct(100), lv_pct(100));
  lv_obj_align(container, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN_WRAP);
  lv_obj_set_flex_align(container, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_radius(container, 0, 0);
  lv_obj_set_style_border_color(container, lv_color_hex(0xeed49f), LV_PART_MAIN);
  lv_obj_set_style_border_width(container, 1, LV_PART_MAIN);
  lv_obj_set_style_bg_color(container, lv_color_hex(0x181926), LV_PART_MAIN);
  lv_obj_set_style_opa(container, LV_OPA_100, LV_PART_MAIN);
  lv_obj_set_style_pad_all(container, 2, LV_PART_MAIN);

  // Create instruction label
  lv_obj_t *instructionLabel = lv_label_create(container);
  lv_obj_set_width(instructionLabel, lv_pct(100));
  lv_label_set_long_mode(instructionLabel, LV_LABEL_LONG_WRAP);
  lv_label_set_text(instructionLabel, "Scan this first QR code to join the configuration network.");
  lv_obj_set_style_text_align(instructionLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_obj_set_style_text_color(instructionLabel, lv_color_hex(0xcad3f5), LV_PART_MAIN);
  lv_obj_set_style_border_color(instructionLabel, lv_color_hex(0xeed49f), LV_PART_MAIN);
  lv_obj_set_style_border_width(instructionLabel, 1, LV_PART_MAIN);
  lv_obj_set_style_text_font(instructionLabel, &lv_font_montserrat_12_subpx, LV_PART_MAIN);

  // Create QR code
  lv_obj_t *qrcode = lv_qrcode_create(container, 150, lv_color_hex(0x181926), lv_color_hex(0xcad3f5));
  lv_qrcode_update(qrcode, qrData.c_str(), qrData.length());
  lv_obj_set_style_border_color(qrcode, lv_color_hex(0xeed49f), LV_PART_MAIN);
  lv_obj_set_style_border_width(qrcode, 1, LV_PART_MAIN);

  // TODO: Create tui-spinner ◐ ◓ ◑ ◒

  // TODO: Create network label
  lv_obj_t *networkLabel = lv_label_create(container);
  lv_label_set_text(networkLabel, "Waiting for connection");
}

void NetworkSetup::display_config_form_QR() {}

void NetworkSetup::launch_webserver() {
  server.on("/config", HTTP_GET, [this]() {
    String html =
        "<form method='POST' action='/save'>"
        "<input type='text' name='ssid' placeholder='WiFi SSID'><br>" // TODO: Replace with dropdown of available networks from wifi_scan()
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
