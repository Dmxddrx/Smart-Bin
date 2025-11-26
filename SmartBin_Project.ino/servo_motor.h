#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#include <Arduino.h>

#define SERVO_PIN 3
#define SERVO_CLOSED_POS 135  // adjust this to your actual closed angle (0–180)
#define SERVO_OPEN_POS   0   // open position

void Servo_init();
void Servo_openLid();
void Servo_closeLid();

#endif
