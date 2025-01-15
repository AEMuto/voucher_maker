#pragma once
#include <Arduino.h>

inline String pwdGen(int length) {
  const char characters[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz"
      "0123456789"
      "!@#$%^&*()-_=+[]{}|;:,.<>?";

  size_t charCount = sizeof(characters) - 1;  // Exclude null terminator
  String password;
  for (int i = 0; i < length; i++) {
    password += characters[random(0, charCount)];
  }
  return password;
}