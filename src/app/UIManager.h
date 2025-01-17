#pragma once

#include <TFT_eSPI.h>
#include <lvgl.h>

#include "Model.h"
#include "globals.h"

#define SCREEN_WIDTH 170
#define SCREEN_HEIGHT 320
#define BUFFER_SIZE SCREEN_WIDTH *SCREEN_HEIGHT / 10  // increase if you need smoother updates

class UIManager : public Observer {
 private:
  Model &model;
  static TFT_eSPI tft;
  static lv_color_t buf[BUFFER_SIZE];
  static lv_disp_draw_buf_t draw_buf;
  // Current UI state tracking
  lv_obj_t *currentScreen = nullptr;
  lv_obj_t *currentMenu = nullptr;
  int currentMenuIndex = 0;
  int currentTestScreen = 0;
  // An array of screens for testing
  array<lv_obj_t *, 3> screens;

 public:
  UIManager(Model &model);
  ~UIManager() override;
  void onModelChanged(const ModelEventData &event) override;
  void begin();
  void update();

 private:
  static void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

  // From observer
  void handleNetworkStateChange(NetworkState newState);
  void handleAppStateChange(AppState newState);
  void handleErrors(ErrorState newState);

  // UI state management
  void handleButtonPress(ButtonAction action);
  void navigateMenu(ButtonAction action);
  void handleMenuSelection();
  void goBack();
  void testScreens(ButtonAction action);
};