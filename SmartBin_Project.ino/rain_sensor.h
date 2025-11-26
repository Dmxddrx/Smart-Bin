#ifndef RAIN_SENSOR_H
#define RAIN_SENSOR_H

#define RAIN_ANALOG_PIN A0
#define RAIN_DIGITAL_PIN 4

// Threshold for dry/wet decision
#define RAIN_WET_THRESHOLD 300  // > 300 = dry, <= 300 = wet

void RainSensor_init();
int  RainSensor_readAnalog();   // 0..1023
bool RainSensor_isWet();        // true if wet (based on threshold)

#endif
