#include "stepper_motor.h"
#include <Arduino.h>

// 28BYJ-48 with common values (gearbox included). Many implementations use 4096 steps/rev (half-step)
#define STEPPER_STEPS_PER_REV 4096L
// We'll keep a small step delay (ms) for reasonable speed
#define STEPPER_DEFAULT_STEP_DELAY 2

// half-step sequence (8-step)
static const uint8_t seq[8][4] = {
  {1,0,0,0},
  {1,1,0,0},
  {0,1,0,0},
  {0,1,1,0},
  {0,0,1,0},
  {0,0,1,1},
  {0,0,0,1},
  {1,0,0,1}
};

static long currentStep = 0; // 0..STEPS_PER_REV-1 : absolute position in steps

void Stepper_init() {
  pinMode(STEPPER_IN1, OUTPUT);
  pinMode(STEPPER_IN2, OUTPUT);
  pinMode(STEPPER_IN3, OUTPUT);
  pinMode(STEPPER_IN4, OUTPUT);
  // ensure off
  digitalWrite(STEPPER_IN1, LOW);
  digitalWrite(STEPPER_IN2, LOW);
  digitalWrite(STEPPER_IN3, LOW);
  digitalWrite(STEPPER_IN4, LOW);
  currentStep = 0;
  delay(10);
}

void Stepper_writePhase(int phase) {
  digitalWrite(STEPPER_IN1, seq[phase][0]);
  digitalWrite(STEPPER_IN2, seq[phase][1]);
  digitalWrite(STEPPER_IN3, seq[phase][2]);
  digitalWrite(STEPPER_IN4, seq[phase][3]);
}

// step is relative count of half-steps
void Stepper_step(int step) {
  // move one half-step at a time and update currentStep
  // positive step -> forward, negative -> backward
  if (step > 0) {
    for (int i = 0; i < step; i++) {
      // compute phase index from currentStep
      int phase = (currentStep % 8 + 8) % 8;
      Stepper_writePhase(phase);
      delay(STEPPER_DEFAULT_STEP_DELAY);
      currentStep = (currentStep + 1) % STEPPER_STEPS_PER_REV;
    }
  } else if (step < 0) {
    for (int i = 0; i < -step; i++) {
      int phase = (currentStep % 8 + 8) % 8;
      Stepper_writePhase(phase);
      delay(STEPPER_DEFAULT_STEP_DELAY);
      // step backwards
      currentStep = (currentStep - 1 + STEPPER_STEPS_PER_REV) % STEPPER_STEPS_PER_REV;
    }
  }
  // After motion, disable coils to reduce heating
  digitalWrite(STEPPER_IN1, LOW);
  digitalWrite(STEPPER_IN2, LOW);
  digitalWrite(STEPPER_IN3, LOW);
  digitalWrite(STEPPER_IN4, LOW);
}

void Stepper_moveSteps(long steps, unsigned int stepDelayMs) {
  // Move 'steps' half-steps. stepDelayMs is ms between phase changes.
  long abssteps = (steps >= 0) ? steps : -steps;
  if (abssteps == 0) return;

  if (stepDelayMs != STEPPER_DEFAULT_STEP_DELAY) {
    // temporary override
  }

  if (steps > 0) {
    for (long i = 0; i < steps; i++) {
      int phase = (currentStep % 8 + 8) % 8;
      Stepper_writePhase(phase);
      delay(stepDelayMs);
      currentStep = (currentStep + 1) % STEPPER_STEPS_PER_REV;
    }
  } else {
    for (long i = 0; i < abssteps; i++) {
      int phase = (currentStep % 8 + 8) % 8;
      Stepper_writePhase(phase);
      delay(stepDelayMs);
      currentStep = (currentStep - 1 + STEPPER_STEPS_PER_REV) % STEPPER_STEPS_PER_REV;
    }
  }

  // turn off coils
  digitalWrite(STEPPER_IN1, LOW);
  digitalWrite(STEPPER_IN2, LOW);
  digitalWrite(STEPPER_IN3, LOW);
  digitalWrite(STEPPER_IN4, LOW);
}

void Stepper_toPositionDegree(int degree) {
  // normalize degree 0..359
  degree = ((degree % 360) + 360) % 360;
  long targetStep = (long) (( (float)STEPPER_STEPS_PER_REV * degree) / 360.0);
  long diff = targetStep - currentStep;
  // choose shortest path:
  if (diff > STEPPER_STEPS_PER_REV/2) diff -= STEPPER_STEPS_PER_REV;
  if (diff < -STEPPER_STEPS_PER_REV/2) diff += STEPPER_STEPS_PER_REV;
  Stepper_moveSteps(diff, STEPPER_DEFAULT_STEP_DELAY);
}

void Stepper_toPosition(int positionIndex) {
  // positionIndex: 0 -> 0°, 1 -> 120°, 2 -> 240°
  int deg = 0;
  switch (positionIndex) {
    case 0: deg = 0; break;
    case 1: deg = 120; break;
    case 2: deg = 240; break;
    default: deg = 0; break;
  }
  Stepper_toPositionDegree(deg);
}
