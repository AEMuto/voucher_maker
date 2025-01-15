#pragma once

#include <globals.h>
#include <TFT_eSPI.h>
#include <lvgl.h>

#include "Model.h"

#define SCREEN_WIDTH 170
#define SCREEN_HEIGHT 320
#define BUFFER_SIZE SCREEN_WIDTH *SCREEN_HEIGHT / 10  // increase if you need smoother updates

class UIManager : public Observer {
 private:
  Model &model;
  static TFT_eSPI tft;
  static lv_color_t buf[BUFFER_SIZE];
  static lv_disp_draw_buf_t draw_buf;

 public:
  UIManager(Model& model);
  ~UIManager() override;
  void onModelChanged(const ModelEventData& event) override;
  void begin();
  void update();

 private:
  static void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
};