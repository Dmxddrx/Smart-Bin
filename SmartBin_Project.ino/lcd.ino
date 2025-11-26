#include "lcd.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 4);  // Most common I2C LCD address

void LCD_init() {
  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0, 0); lcd.print("SmartBin Ready!");
  delay(1000);
  lcd.clear();
}

void LCD_updateStatus(bool wetFull, bool dryFull, bool metalFull,
                      float dWet, float dDry, float dMetal,
                      const char *detectedType,
                      int rainRaw, float ultrasonicCM)
{
  lcd.clear();

  // ----------- Row 1: WET -------------
  lcd.setCursor(0, 0);
  lcd.print("WET:");
  lcd.setCursor(5, 0);
  lcd.print(wetFull ? "FULL " : "EMPTY");

  lcd.setCursor(11, 0);
  lcd.print(dWet, 1);  // 14.5cm

  // ----------- Row 2: DRY -------------
  lcd.setCursor(0, 1);
  lcd.print("DRY:");
  lcd.setCursor(5, 1);
  lcd.print(dryFull ? "FULL " : "EMPTY");

  lcd.setCursor(11, 1);
  lcd.print(dDry, 1);

  // ----------- Row 3: METAL -----------
  //lcd.setCursor(0, 2);
  //lcd.print("MET:");
  lcd.setCursor(1, 2);
  lcd.print(metalFull ? "FULL " : "EMPTY");

  lcd.setCursor(7, 2);
  lcd.print(dMetal, 1);

  // ----------- Row 4: SENSOR STATUS ---
  lcd.setCursor(0, 3);
  lcd.print("DET:");
  lcd.setCursor(5, 3);
  lcd.print(detectedType);
}
