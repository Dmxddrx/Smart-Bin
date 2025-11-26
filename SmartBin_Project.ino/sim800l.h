#ifndef SIM800L_H
#define SIM800L_H

#include <Arduino.h>

#define SIM800L_TX_PIN A3  // SIM800L TX → Arduino A3 (Software RX)
#define SIM800L_RX_PIN A4  // SIM800L RX → Arduino A4 (Software TX, use divider)
#define BAUDRATE 9600

void SIM800L_init();
void SIM800L_sendSMS(const char *number, const char *message);

#endif
