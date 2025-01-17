#include "ButtonManager.h"

ButtonManager::ButtonManager(ActionCallback callback)
    : callback(move(callback)) {
  // Initialize buttons with active LOW configuration
  selectBtn = make_unique<OneButton>(IO01_BTN, true);
  downBtn = make_unique<OneButton>(IO00_BTN, true);
  upBtn = make_unique<OneButton>(IO14_BTN, true);
}

void ButtonManager::begin() {
  // Configure reset button for multiple functions
  selectBtn->attachClick([](void *ctx) { ((ButtonManager *)ctx)->handleSelectClick(); }, this);
  selectBtn->attachLongPressStart([](void *ctx) { ((ButtonManager *)ctx)->handleSelectLongPressStart(); }, this);
  selectBtn->attachLongPressStop([](void *ctx) { ((ButtonManager *)ctx)->handleSelectLongPressStop(); }, this);

  // Configure single-function buttons
  downBtn->attachClick([](void *ctx) { ((ButtonManager *)ctx)->handleDownClick(); }, this);
  upBtn->attachClick([](void *ctx) { ((ButtonManager *)ctx)->handleUpClick(); }, this);

  // Set timing parameters for better response
  selectBtn->setDebounceMs(50);
  selectBtn->setClickMs(600);
  downBtn->setDebounceMs(50);
  upBtn->setDebounceMs(50);
}

void ButtonManager::update() {
  // Serial.println("ButtonManager update");
  selectBtn->tick();
  downBtn->tick();
  upBtn->tick();
  delay(5);

  // Check for very long press (reboot)
  if (isLongPressing) {
    checkLongPressDuration();
  }
}

void ButtonManager::handleSelectClick() {
  if (callback) {
    Serial.println("Select button clicked");
    btnAction = ButtonAction::SELECT;
    callback(btnAction);
  }
}

void ButtonManager::handleSelectLongPressStart() {
  isLongPressing = true;
  longPressStart = millis();
}

void ButtonManager::handleSelectLongPressStop() {
  isLongPressing = false;

  // If we haven't triggered a reboot, it's a back action
  unsigned long pressDuration = millis() - longPressStart;
  if (pressDuration >= BACK_PRESS_MS && pressDuration < REBOOT_PRESS_MS) {
    if (callback) {
      Serial.println("Select button long pressed");
      btnAction = ButtonAction::BACK;
      callback(btnAction);
    }
  }
}

void ButtonManager::checkLongPressDuration() {
  if (!isLongPressing) return;

  unsigned long pressDuration = millis() - longPressStart;
  if (pressDuration >= REBOOT_PRESS_MS) {
    if (callback) {
      Serial.println("Select button very long pressed");
      btnAction = ButtonAction::REBOOT;
      callback(btnAction);
    }
    isLongPressing = false;  // Prevent multiple triggers
  }
}

void ButtonManager::handleDownClick() {
  if (callback) {
    Serial.println("Down button clicked");
    btnAction = ButtonAction::DOWN;
    callback(btnAction);
  }
}

void ButtonManager::handleUpClick() {
  if (callback) {
    Serial.println("Up button clicked");
    btnAction = ButtonAction::UP;
    callback(btnAction);
  }
}