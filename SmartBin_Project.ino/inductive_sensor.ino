#include "inductive_sensor.h"
#include <Arduino.h>

void Inductive_init() {
  // Use internal pullup if sensor is open-collector (NPN)
  if (INDUCTIVE_ACTIVE_LOW) {
    pinMode(INDUCTIVE_PIN, INPUT_PULLUP);
  } else {
    pinMode(INDUCTIVE_PIN, INPUT);
  }
}

bool Inductive_isMetal() {
  int v = digitalRead(INDUCTIVE_PIN);
  if (INDUCTIVE_ACTIVE_LOW) {
    // active low: LOW -> detected
    return (v == HIGH);
  } else {
    return (v == LOW);
  }
}
