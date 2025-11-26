#ifndef BUZZER_H
#define BUZZER_H

#define BUZZER_PIN 12

void Buzzer_init();
void Buzzer_beep(int times, unsigned int durationMs); // times beep, duration each (ms)
void Buzzer_beepPattern_twoPairs(); // helper for the 2-beeps pattern described

#endif
