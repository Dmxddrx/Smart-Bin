#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <Arduino.h>

void LCD_init();
void LCD_updateStatus(bool wetFull, bool dryFull, bool metalFull,
                      float dWet, float dDry, float dMetal,
                      const char *detectedType,
                      int rainRaw, float ultrasonicCM);

#endif
