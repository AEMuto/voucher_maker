#pragma once

#include <globals.h>
#define SCREEN_WIDTH 170
#define SCREEN_HEIGHT 320
#define BUF_SIZE SCREEN_WIDTH * SCREEN_HEIGHT / 10  // increase if you need smoother updates

class UIManager {
 public:
  UIManager();

 private:
  static TFT_eSPI tft;
  static lv_color_t buf[BUF_SIZE];
  static lv_disp_draw_buf_t draw_buf;

  static void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
};