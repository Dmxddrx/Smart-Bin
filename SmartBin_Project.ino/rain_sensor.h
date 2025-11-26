#ifndef RAIN_SENSOR_H
#define RAIN_SENSOR_H

#include <Arduino.h>

// Pin definitions
#define RAIN_ANALOG_PIN   A0
#define RAIN_DIGITAL_PIN  4

// Threshold for wet/dry detection
// NOTE: You can tune this after reading raw values
#define RAIN_WET_THRESHOLD  900   // <= threshold → wet, > threshold → dry

void RainSensor_init();
int  RainSensor_readAnalog();      // returns 0–1023
bool RainSensor_readDigital();     // returns HIGH/LOW
bool RainSensor_isWet();           // true = wet

#endif
