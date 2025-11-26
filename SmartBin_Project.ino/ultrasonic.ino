#include "ultrasonic.h"
#include <Arduino.h>

void Ultrasonic_init() {
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  delay(5);
}

float Ultrasonic_readCM() {
  // Send trigger pulse
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH, 30000); // 30ms timeout
  if (duration == 0) return 999.0; // timeout -> treat as far/invalid

  // Speed of sound ~343 m/s = 0.0343 cm/us
  float cm = (duration * 0.0343) / 2.0;
  return cm;
}
