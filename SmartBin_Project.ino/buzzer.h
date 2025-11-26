#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

// Define buzzer pin here
#define BUZZER_PIN A1  // change to the pin you want (analog pin can be used as digital)

// Initialize buzzer pin
void Buzzer_init();

// Simple beep function
// times = number of beeps
// durationMs = duration of each beep in milliseconds
void Buzzer_beep(int times, unsigned int durationMs);

// Pattern: 2 beeps repeated 3 times with 500ms delay
void Buzzer_beepPattern_twoPairs();

#endif
