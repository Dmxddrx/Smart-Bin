#ifndef LEDS_H
#define LEDS_H

#define LED_METAL_PIN 13
#define LED_DRY_PIN A1
#define LED_WET_PIN A2

void LED_init();
void LED_on(int id);   // 1=metal,2=dry,3=wet
void LED_off(int id);
void LED_blink(int id, int times); // blink with 300ms per blink (on/off)
void LED_allBlinkOnce();

#endif
