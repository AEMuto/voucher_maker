#include <Arduino.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <nvs_flash.h>

#include "NetworkSetup.h"
#include "tdisplay_config.h"

#define BUF_SIZE SCREEN_WIDTH *SCREEN_HEIGHT / 10  // larger buffer for smoother updates

static lv_color_t buf[BUF_SIZE];
static lv_disp_draw_buf_t draw_buf;

TFT_eSPI tft = TFT_eSPI();  // Display driver object representation
NetworkSetup network;            // Instanciate WifiConfig object

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

void setup() {
  /* (POWER ON)IO15 must be set to HIGH before starting,
  /* otherwise the screen will not display when using battery */
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);

  Serial.begin(115200);

  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  lv_init();          // Initialize LittlevGL
  tft.begin();        // Initialize the display
  tft.setRotation(0); /* Portrait orientation */

  // Initialize buffer
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, BUF_SIZE);

  /* Initialize the display driver for LittlevGL */
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = SCREEN_WIDTH;
  disp_drv.ver_res = SCREEN_HEIGHT;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /* Initialize the (dummy) input device driver for LittlevGL */
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  lv_indev_drv_register(&indev_drv);

  // Fill screen with a color to see boundaries
  lv_obj_t *screen = lv_scr_act();
  lv_obj_set_style_bg_color(screen, lv_color_hex(0x181926), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(screen, LV_OPA_100, LV_PART_MAIN | LV_STATE_DEFAULT);

  // If there is no configured WiFi, start the configuration portal
  if (!network.is_configured()) {
    network.start_config();
  } else {
    network.connect_to_wan();
  }
}

void loop() {
  // Handle WiFi configuration
  if (!network.is_configured()) {
    network.handle_webclient();
  }
  // Handle LittlevGL tasks
  lv_task_handler();
  delay(5);
  if (WiFi.status() == WL_CONNECTED) {
    // Do something when connected
  } else {
    // try to reconnect with the main WiFi network
  }
}