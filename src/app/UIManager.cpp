#include "UIManager.h"

TFT_eSPI UIManager::tft = TFT_eSPI();
lv_disp_draw_buf_t UIManager::draw_buf;
lv_color_t UIManager::buf[BUFFER_SIZE];

UIManager::UIManager(Model &model) : model(model) {
  model.addObserver(this);
}

UIManager::~UIManager() {
  model.removeObserver(this);
}

void UIManager::onModelChanged(const ModelEventData &event) {
  switch (event.event) {
    case ModelEvent::APP_STATE_CHANGED:
      // Handle app state change
      handleAppStateChange(*static_cast<AppState *>(event.data));
      break;
    case ModelEvent::NETWORK_STATUS_CHANGED:
      // Handle network state change
      handleNetworkStateChange(*static_cast<NetworkState *>(event.data));
      break;
    case ModelEvent::ERROR_OCCURRED:
      // Handle error event
      handleErrors(*static_cast<ErrorState *>(event.data));
      break;
    case ModelEvent::BUTTON_PRESSED:
      auto *action = static_cast<ButtonAction *>(event.data);
      handleButtonPress(*action);
      break;
  }
}

void UIManager::begin() {
  lv_init();          // Initialize LVGL
  tft.begin();        // Initialize the display
  tft.setRotation(0); /* Portrait orientation */

  // Initialize buffer
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, BUFFER_SIZE);

  /* Initialize the display driver for LVGL */
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = SCREEN_WIDTH;
  disp_drv.ver_res = SCREEN_HEIGHT;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /* Initialize the (dummy) input device driver for LVGL */
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  lv_indev_drv_register(&indev_drv);

  // Populate test screens array
  for (int i = 0; i < screens.size(); i++) {
    screens[i] = lv_obj_create(NULL);
    uint32_t bg_color = i % 3 == 0 ? 0xFF0000 : i % 3 == 1 ? 0x00FF00 : 0x0000FF;
    lv_obj_set_style_bg_color(screens[i], lv_color_hex(bg_color), LV_PART_MAIN);
    lv_obj_t *label = lv_label_create(screens[i]);
    String screenText = "Screen " + String(i);
    lv_label_set_text(label, screenText.c_str());
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0); 
  }
  lv_scr_load(screens[currentTestScreen]);
}

void UIManager::update() {
  lv_task_handler();  // Call the LVGL task handler
  delay(5);           // Delay to allow ESP32 to handle other tasks
}

// Necessary method to flush the LVGL buffer to the TFT display
void UIManager::my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  // Start writing to the TFT display
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  // Inform LVGL that the flushing is done
  lv_disp_flush_ready(disp);
}

void UIManager::handleButtonPress(ButtonAction action) {
  switch (action) {  // Note: dereferencing pointer now
    case ButtonAction::UP:
    case ButtonAction::DOWN:
      Serial.println("From UIManager");
      Serial.printf("Button %s pressed\n", action == ButtonAction::UP ? "UP" : "DOWN");
      // navigateMenu(action);
      testScreens(action);
      break;
    case ButtonAction::SELECT:
      Serial.println("From UIManager");
      Serial.println("Select button pressed");
      // handleMenuSelection();
      break;
    case ButtonAction::BACK:
      Serial.println("From UIManager");
      Serial.println("Back button pressed");
      // goBack();
      break;
    case ButtonAction::REBOOT:
      // Handle reboot UI if needed
      break;
  }
}

void UIManager::testScreens(ButtonAction action) {
  // Cycle through test screens
  if (action == ButtonAction::UP) {
    currentTestScreen = (currentTestScreen + 1) % screens.size();
  } else if (action == ButtonAction::DOWN) {
    currentTestScreen = (currentTestScreen - 1 + screens.size()) % screens.size();
  }

  // Show the selected screen
  lv_scr_load(screens[currentTestScreen]);
}

void UIManager::handleNetworkStateChange(NetworkState newState) {
  // React to network state changes
  switch (newState) {
    case NetworkState::DISCONNECTED:
      // If model has wan credentials, attempt to connect else set up AP, web server etc.
      break;
    case NetworkState::AP_MODE:
      // Set up AP if not already done
      break;
    case NetworkState::WAITING_FOR_CLIENT:
      // AP is active, waiting for user to connect
      break;
    case NetworkState::CLIENT_CONNECTED:
      // User has connected to our AP
      break;
    case NetworkState::AWAITING_CREDENTIALS:
      // Waiting for user to submit credentials
      break;
    case NetworkState::CONNECTING:
      // Attempt to connect to main network
      break;
    case NetworkState::CONNECTION_FAILED:
      // Connection attempt failed
      break;
    case NetworkState::CONNECTED:
      // Transition to main network
      break;
    default:
      break;
  }
}

void UIManager::handleAppStateChange(AppState newState) {
  // React to app state changes
  switch (newState) {
    case AppState::BOOT:
      // Display idle screen
      break;
    case AppState::NETWORK_SETUP:
      // Display configuration screen
      break;
    case AppState::ERROR:
      // Display error screen
      break;
    default:
      break;
  }
}

void UIManager::handleErrors(ErrorState newState) {
  // React to error state changes
  switch (newState) {
    case ErrorState::NONE:
      // Clear error message
      break;
    case ErrorState::NETWORK_UNREACHABLE:
      // Display network unreachable message
      break;
    case ErrorState::SERVER_UNREACHABLE:
      // Display server unreachable message
      break;
    case ErrorState::INVALID_CREDENTIALS:
      // Display invalid credentials message
      break;
    case ErrorState::MQTT_ERROR:
      // Display MQTT error message
      break;
    case ErrorState::PRINTER_ERROR:
      // Display printer error message
      break;
    case ErrorState::CRITICAL_ERROR:
      // Display critical error message
      break;
    default:
      break;
  }
}

void UIManager::navigateMenu(ButtonAction action) {
  if (!currentMenu) return;

  uint16_t itemCount = lv_obj_get_child_cnt(currentMenu);
  if (itemCount == 0) return;

  // Update selection based on action
  if (action == ButtonAction::UP) {
    currentMenuIndex = (currentMenuIndex - 1 + itemCount) % itemCount;
  } else if (action == ButtonAction::DOWN) {
    currentMenuIndex = (currentMenuIndex + 1) % itemCount;
  }

  // Update UI to show selection
  lv_obj_t *selectedItem = lv_obj_get_child(currentMenu, currentMenuIndex);
  if (selectedItem) {
    // Update styling for selected item
    lv_obj_add_state(selectedItem, LV_STATE_FOCUSED);
    // Remove focus from siblings
    for (int i = 0; i < itemCount; i++) {
      if (i != currentMenuIndex) {
        lv_obj_t *item = lv_obj_get_child(currentMenu, i);
        lv_obj_clear_state(item, LV_STATE_FOCUSED);
      }
    }
  }
}

void UIManager::handleMenuSelection() {
  if (!currentMenu) return;

  lv_obj_t *selectedItem = lv_obj_get_child(currentMenu, currentMenuIndex);
  if (selectedItem) {
    // // Get menu item ID or user data to determine action
    // uint32_t itemId = lv_obj_get_user_data(selectedItem);

    // // Handle different menu actions based on itemId
    // switch (itemId) {
    //   case MENU_VIEW_VOUCHERS:
    //     model->setAppState(AppState::VIEWING_VOUCHERS);
    //     break;
    //   case MENU_GENERATE_VOUCHER:
    //     model->setAppState(AppState::GENERATING_VOUCHER);
    //     break;
    //     // ... other menu actions ...
  }
}

void UIManager::goBack() {
  // Handle navigation back based on current screen
  switch (model.getAppState()) {
    case AppState::VIEWING_VOUCHERS:
    case AppState::GENERATING_VOUCHER:
      model.setAppState(AppState::READY);
      break;
      // ... handle other states ...
  }
}