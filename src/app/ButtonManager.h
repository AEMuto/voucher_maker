#pragma once

#include <OneButton.h>

#include <array>
#include <functional>
#include <memory>

#include "globals.h"
#include "pins.h"

class ButtonManager {
 public:
  using ActionCallback = function<void(ButtonAction)>;

  ButtonManager(ActionCallback callback);
  ~ButtonManager() = default;

  void begin();
  void update();

 private:
  ButtonAction btnAction = ButtonAction::NONE;
  static constexpr uint16_t BACK_PRESS_MS = 3000;     // 3s for back
  static constexpr uint16_t REBOOT_PRESS_MS = 10000;  // 10s for reboot

  // Keep track of all buttons
  unique_ptr<OneButton> selectBtn;  // Multiple functions button (select, back, reboot)
  unique_ptr<OneButton> downBtn;    // Single function button (boot)
  unique_ptr<OneButton> upBtn;      // Single function button (IO14)

  ActionCallback callback;
  bool isLongPressing = false;
  unsigned long longPressStart = 0;

  // Button event handlers
  void handleSelectClick();
  void handleSelectLongPressStart();
  void handleSelectLongPressStop();
  void handleDownClick();
  void handleUpClick();

  // Helper to check long press duration
  void checkLongPressDuration();
};