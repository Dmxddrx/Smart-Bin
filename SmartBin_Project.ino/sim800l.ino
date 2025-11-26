#include <SoftwareSerial.h>
#include "sim800l.h"

// SoftwareSerial object for GSM
SoftwareSerial sim800l(SIM800L_TX_PIN, SIM800L_RX_PIN); // RX, TX

void SIM800L_init() {
  sim800l.begin(BAUDRATE);
  delay(1000);
  Serial.println("SIM800L initialized.");

  // Optional AT check
  sim800l.println("AT");
  delay(500);
  while (sim800l.available()) {
    Serial.write(sim800l.read());
  }
}

void SIM800L_sendSMS(const char *number, const char *message) {
  sim800l.println("AT+CMGF=1"); // Text mode
  delay(500);

  sim800l.print("AT+CMGS=\"");
  sim800l.print(number);
  sim800l.println("\"");
  delay(500);

  sim800l.print(message);
  delay(200);

  sim800l.write(26); // Ctrl+Z
  delay(5000);

  Serial.println("SMS sent:");
  Serial.println(message);
}
