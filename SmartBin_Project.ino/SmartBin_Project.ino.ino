// Main sketch: SmartBin_Project.ino
// Combines all modules and runs the system loop.

#include "ultrasonic.h"
#include "rain_sensor.h"
#include "inductive_sensor.h"
#include "ir_sensor.h"
#include "servo_motor.h"
#include "stepper_motor.h"
#include "buzzer.h"
#include "leds.h"
#include "logic_controller.h"
#include "sim800l.h"
#include "lcd.h"

void setup() {
  Serial.begin(9600);

  LCD_init();
  Ultrasonic_init();
  RainSensor_init();
  Inductive_init();
  IR_init();
  Servo_init();
  Stepper_init();
  Buzzer_init();
  LED_init();
  SIM800L_init(); // initialize GSM module

  System_init(); // initialize logic controller state
}

void loop() {
  System_mainLoop(); // main behaviour (blocking style per your spec)
}
