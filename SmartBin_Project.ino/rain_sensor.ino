#include "rain_sensor.h"

// Initialize sensor pins
void RainSensor_init() {
    pinMode(RAIN_DIGITAL_PIN, INPUT);
}

// Read analog value (0–1023)
int RainSensor_readAnalog() {
    return analogRead(RAIN_ANALOG_PIN);
}

// Read digital pin (optional)
bool RainSensor_readDigital() {
    return digitalRead(RAIN_DIGITAL_PIN);   // HIGH or LOW
}

// Determine wet/dry condition using analog value
bool RainSensor_isWet() {
    int val = RainSensor_readAnalog();
    return (val <= RAIN_WET_THRESHOLD);
}
