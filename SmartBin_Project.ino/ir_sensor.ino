#include "ir_sensor.h"
#include <Arduino.h>

void IR_init() {
  pinMode(IR_PIN, INPUT);
}

bool IR_detectObject() {
  // Many IR modules produce LOW when object is detected or HIGH depending on module.
  // We'll assume HIGH means object detected (if inverted, change logic or set variable).
  int v = digitalRead(IR_PIN);
  return (v == LOW);
}
