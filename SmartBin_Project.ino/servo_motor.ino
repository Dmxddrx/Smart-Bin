#include "servo_motor.h"
#include <Servo.h>

static Servo myServo;

void Servo_init() {
  // set pin LOW before attaching (prevent early signal noise)
  pinMode(SERVO_PIN, OUTPUT);
  digitalWrite(SERVO_PIN, LOW);
  delay(100);

  myServo.attach(SERVO_PIN);

  // move gently to closed position after short delay
  delay(200);
  myServo.write(SERVO_CLOSED_POS);
  delay(300);

  // optional: detach servo after positioning to prevent jitter
  myServo.detach();
}

void Servo_openLid() {
  myServo.attach(SERVO_PIN);
  myServo.write(SERVO_OPEN_POS);
  delay(400);
  myServo.detach();
}

void Servo_closeLid() {
  myServo.attach(SERVO_PIN);
  myServo.write(SERVO_CLOSED_POS);
  delay(400);
  myServo.detach();
}
