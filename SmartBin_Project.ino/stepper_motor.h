#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

#define STEPPER_IN1 11
#define STEPPER_IN2 10
#define STEPPER_IN3 9
#define STEPPER_IN4 8

void Stepper_init();
void Stepper_step(int step); // single step relative: positive = forward steps, negative = back
void Stepper_moveSteps(long steps, unsigned int stepDelayMs); // blocking, moves given steps
void Stepper_toPositionDegree(int degree); // moves to absolute degree (0,120,240) relative to home
void Stepper_toPosition(int positionIndex); // positionIndex 0->0°, 1->120°, 2->240°

#endif
