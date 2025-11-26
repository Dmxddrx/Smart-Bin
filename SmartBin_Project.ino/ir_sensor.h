#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#define IR_PIN 2

void IR_init();
bool IR_detectObject(); // returns true if object detected (IR module output active)

#endif
