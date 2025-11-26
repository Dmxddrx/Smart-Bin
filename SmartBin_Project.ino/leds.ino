#include "leds.h"
#include <Arduino.h>

void LED_init() {
  pinMode(LED_METAL_PIN, OUTPUT);
  pinMode(LED_DRY_PIN, OUTPUT);
  pinMode(LED_WET_PIN, OUTPUT);
  digitalWrite(LED_METAL_PIN, LOW);
  digitalWrite(LED_DRY_PIN, LOW);
  digitalWrite(LED_WET_PIN, LOW);
}

static int idToPin(int id) {
  switch (id) {
    case 1: return LED_METAL_PIN;
    case 2: return LED_DRY_PIN;
    case 3: return LED_WET_PIN;
    default: return -1;
  }
}

void LED_on(int id) {
  int p = idToPin(id);
  if (p < 0) return;
  digitalWrite(p, HIGH);
}

void LED_off(int id) {
  int p = idToPin(id);
  if (p < 0) return;
  digitalWrite(p, LOW);
}

void LED_blink(int id, int times) {
  int p = idToPin(id);
  if (p < 0) return;
  for (int i = 0; i < times; i++) {
    digitalWrite(p, HIGH);
    delay(300); // on 300ms
    digitalWrite(p, LOW);
    delay(300); // off 300ms between blink pairs (per your spec)
  }
}

void LED_allBlinkOnce() {
  LED_blink(1,1);
  LED_blink(2,1);
  LED_blink(3,1);
}
