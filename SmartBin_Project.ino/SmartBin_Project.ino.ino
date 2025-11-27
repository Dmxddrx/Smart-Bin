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

  LCD_init(); // initialize LCD Display
  Ultrasonic_init(); // initialize ultrasonic
  RainSensor_init(); // initialize rain sensor
  Inductive_init(); // initialize Metal detector
  IR_init(); // initialize IR
  Servo_init(); // initialize Servo motor
  Stepper_init(); // initialize Stepper motor
  Buzzer_init(); // initialize Buzzer
  LED_init(); // initialize LEDs
  SIM800L_init(); // initialize GSM module

  System_init(); // initialize logic controller state
}

void loop() {
  System_mainLoop();
}
