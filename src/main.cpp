#include "app/App.h"

App* app = nullptr;

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));  // Initialize random seed
  app = new App();
  app->begin();
}

void loop() {
  app->loop();
}