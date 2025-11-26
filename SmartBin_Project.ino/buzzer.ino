#include "buzzer.h"
#include <Arduino.h>

void Buzzer_init() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

// all beeps are 300ms by system spec; but function accepts duration
void Buzzer_beep(int times, unsigned int durationMs) {
  for (int i = 0; i < times; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(durationMs);
    digitalWrite(BUZZER_PIN, LOW);
    // spacing between beeps per your spec: 300ms between beeps/pairs unless otherwise noted
    delay(300);
  }
}

// pattern: "if not any object detect 2 beeps 500ms delay again 2 beeps 500ms delay again 2 beeps 500ms delay."
void Buzzer_beepPattern_twoPairs() {
  for (int r = 0; r < 3; r++) {
    Buzzer_beep(2, 300);
    delay(500);
  }
}
