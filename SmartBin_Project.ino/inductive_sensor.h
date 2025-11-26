#ifndef INDUCTIVE_SENSOR_H
#define INDUCTIVE_SENSOR_H

#define INDUCTIVE_PIN 5
// If your sensor is active LOW (NPN open-collector) set to true:
#define INDUCTIVE_ACTIVE_LOW true

void Inductive_init();
bool Inductive_isMetal(); // returns true when metal detected

#endif
