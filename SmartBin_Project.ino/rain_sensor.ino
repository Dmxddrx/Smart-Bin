#include "rain_sensor.h"
#include <Arduino.h>

//#define RAIN_WET_THRESHOLD 200  // tune this value; > threshold => wet (example)

// Initialize sensor pins
void RainSensor_init() {
  pinMode(RAIN_DIGITAL_PIN, INPUT); // analog pin needs no pinMode
}

// Read analog value from sensor
int RainSensor_readAnalog() {
  return analogRead(RAIN_ANALOG_PIN);
}

// Determine if wet based on threshold
bool RainSensor_isWet() {
  int val = RainSensor_readAnalog();
  return val <= RAIN_WET_THRESHOLD; // <= threshold = wet
}
