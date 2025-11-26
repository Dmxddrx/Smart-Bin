#ifndef ULTRASONIC_H
#define ULTRASONIC_H

// Pins (match mapping from conversation)
#define ULTRASONIC_TRIG_PIN 7
#define ULTRASONIC_ECHO_PIN 6

void Ultrasonic_init();
float Ultrasonic_readCM(); // returns distance in cm, or 999.0 on timeout

#endif
