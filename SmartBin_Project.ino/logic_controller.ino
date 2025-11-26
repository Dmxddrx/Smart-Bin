#include "logic_controller.h"
#include "ultrasonic.h"
#include "rain_sensor.h"
#include "inductive_sensor.h"
#include "ir_sensor.h"
#include "servo_motor.h"
#include "stepper_motor.h"
#include "buzzer.h"
#include "leds.h"
#include "sim800l.h"
#include "lcd.h"
#include <Arduino.h>

// --- Phone number ---
const char PHONE_NUMBER[] = "+94717418097"; // your phone number

// bin indexes
#define BIN_WET   0
#define BIN_DRY   1
#define BIN_METAL 2

// Depth thresholds (cm) from your spec:
#define BIN_EMPTY_DISTANCE 12   // empty when ~11cm
#define BIN_FULL_DISTANCE 10     // full when < 6cm

static float binDepth[3];
static bool binFull[3];
//static bool binWasFull[3]; // NEW: Tracks the state of the previous loop iteration

// 🔹 Forward declare 
void System_checkFullBins();
void handleGarbageType(int type);
//void sendBinFullSMS(int type);

void System_init() {
  Serial.begin(9600); // Removed: Already called in main sketch setup()
  Serial.println(F("\n[SmartBin] System Initializing..."));

  //SIM800L_init(); // Removed: Already called in main sketch setup()

  // initialize bin depths default (assume empty)
  for (int i = 0; i < 3; i++) {
    binDepth[i] = BIN_EMPTY_DISTANCE;
    binFull[i] = false;
    //binWasFull[i] = false; // Initialize previous state tracker
  }

  // initial full-check
  delay(100);
  System_checkFullBins();

  // indicate ready
  LED_allBlinkOnce();
  //Buzzer_beep(1, 300);

  Serial.println(F("[SmartBin] Initialization Complete ✅"));
}

void System_mainLoop() {
  // Continuously monitor: This high-level loop follows your described flow:
  // 1. Always check metal detection first.
  // 2. If metal -> handle metal flow; else check wet/dry and handle.
  // 3. Each handle returns to begin.

// Step 1: Metal check
  bool isMetal = Inductive_isMetal();
  bool isWet = false;

  if (isMetal) {
    Serial.println(F("[Logic] Metal detected ⚙️"));
    handleGarbageType(BIN_METAL);
  } else {
    // Step 2: Wet/Dry check
    isWet = RainSensor_isWet();
    if (isWet) {
      Serial.println(F("[Logic] Wet waste detected 💧"));
      handleGarbageType(BIN_WET);
    } else {
      Serial.println(F("[Logic] Dry waste detected 🌿"));
      handleGarbageType(BIN_DRY);
    }
  }

  // NOTE: Fullness check must be done periodically. We can call it here 
  // after the garbage drop cycle to ensure state is up-to-date.
  //System_checkFullBins();

  // Debug print for continuous monitoring
  Serial.println(F("────────────────────────────"));
  Serial.print(F("[Monitor] Ultrasonic Distance (cm): "));
  Serial.println(Ultrasonic_readCM());

  // Rain sensor readings
  int rainRaw = RainSensor_readAnalog();
  Serial.print(F("[Monitor] Rain Sensor RAW: "));
  Serial.println(rainRaw);
  Serial.print(F("[Monitor] Rain Sensor: "));
  Serial.println(isWet ? F("WET") : F("DRY"));


  Serial.print(F("[Monitor] IR Object: "));
  Serial.println(IR_detectObject() ? F("OBJECT DETECTED") : F("NO OBJECT"));
  Serial.print(F("[Monitor] Metal: "));
  Serial.println(isMetal ? F("YES") : F("NO"));

    // ✅ Print full/empty status for all bins
  Serial.println(F("------ Bin Status ------"));
  Serial.print(F("Wet bin: "));
  Serial.println(binFull[BIN_WET] ? "FULL" : "EMPTY");
  Serial.print(F("Dry bin: "));
  Serial.println(binFull[BIN_DRY] ? "FULL" : "EMPTY");
  Serial.print(F("Metal bin: "));
  Serial.println(binFull[BIN_METAL] ? "FULL" : "EMPTY");
  Serial.println(F("------------------------"));

  Serial.println(F("────────────────────────────\n"));
  delay(300);

  Serial.println(F("────────────────────────────\n"));

    // LCD update
  const char *detectedLabel = 
      isMetal ? "METAL" : (isWet ? "WET" : "DRY");

  LCD_updateStatus(
    binFull[BIN_WET],
    binFull[BIN_DRY],
    binFull[BIN_METAL],
    binDepth[BIN_WET],
    binDepth[BIN_DRY],
    binDepth[BIN_METAL],
    detectedLabel,
    analogRead(A0),           // rain raw value
    Ultrasonic_readCM()       // live ultrasonic value
  );

  delay(300);
}

/////////////////////////
// Helper functions
/////////////////////////

void System_checkFullBins() {
  // Start at 0° (wet bin)
  Stepper_toPosition(0);
  delay(200);
  float d0 = Ultrasonic_readCM();
  if (d0 < 999.0) binDepth[BIN_WET] = d0;
  //Buzzer_beep(1, 300);

  // rotate +120° -> dry
  Stepper_toPosition(1);
  delay(200);
  float d1 = Ultrasonic_readCM();
  if (d1 < 999.0) binDepth[BIN_DRY] = d1;
  //Buzzer_beep(1, 300);

  // rotate +120° -> metal (240°)
  Stepper_toPosition(2);
  delay(200);
  float d2 = Ultrasonic_readCM();
  if (d2 < 999.0) binDepth[BIN_METAL] = d2;
  //Buzzer_beep(1, 300);

  // return to 0° home
  Stepper_toPosition(0);

  // evaluate fullness
  for (int i = 0; i < 3; i++) {
    binFull[i] = (binDepth[i] < BIN_FULL_DISTANCE);
      if (binFull[i]) {
        sendBinFullSMS(i);
      
      //binWasFull[i] = binFull[i]; // update previous state
      }
  }


  // Evaluate fullness + send SMS if just became full
//  for (int i = 0; i < 3; i++) {
   // bool wasFull = binWasFull[i];
   // binFull[i] = (binDepth[i] < BIN_FULL_DISTANCE);

   // if (!wasFull && binFull[i]) {
    //  sendBinFullSMS(i);
    //}
    //binWasFull[i] = binFull[i];
  //}


  // flash LEDs & beep once to indicate completion
  LED_allBlinkOnce();
  //Buzzer_beep(1, 300);

    // Print bin status
  Serial.println(F("------ Bin Status ------"));
  Serial.print(F("Wet bin: "));
  Serial.println(binFull[BIN_WET] ? "FULL" : "EMPTY");  // no F() here
  Serial.print(F("Dry bin: "));
  Serial.println(binFull[BIN_DRY] ? "FULL" : "EMPTY");  // no F() here
  Serial.print(F("Metal bin: "));
  Serial.println(binFull[BIN_METAL] ? "FULL" : "EMPTY"); // no F() here
  Serial.println(F("------------------------"));


}

// Forward declaration for handler used in loop
void handleGarbageType(int type);

void handleGarbageType(int type) {
  // If full: beep pattern and blink logic until IR says no object (per your spec)
  if (binFull[type]) {
    // three beeps once and then blink/beep repeatedly until IR detects no object
    //Buzzer_beep(3, 300);      // 3 beeps once
    // LED off for that type (spec said LED off)
    LED_off(type + 1);

    // Blink 3 times with beeps until IR HW201 check for object and tell no object
    // Interpretation: Wait while the divider still has an object (IR inside divider says object present),
    // once divider is empty (no object), return to metal detection.
    int attempts = 0;
    while (true) {
      // blink three times with beeps
      for (int b=0; b<3; b++) {
        LED_blink(type + 1, 1); // 1 blink = on 300ms off 300ms
        //Buzzer_beep(1, 300);
        delay(300); // spacing between blink/beep per your spec
      }

      // Check IR in divider (IR_detectObject()).
      // The spec: blinking until IR HW201 check for object and tell no object -> so break when IR reports NO object.
      if (!IR_detectObject()) {
        break;
      }

      // Safety: avoid infinite lock if IR permanently sees object -> add small delay and continue
      attempts++;
      delay(300);
      if (attempts > 50) {
        // if stuck too long, refresh fullness check and break
        System_checkFullBins();
        break;
      }
    }
    // then return to top. (LED already off)
    return;
  }

  // Not full: proceed to drop
  // LED for detected type lights up until told off (per spec)
  LED_on(type + 1);

  // rotate stepper to the correct position:
  //switch (type) {
    //case BIN_WET: Stepper_toPosition(0); break;   // wet = 0°
    //case BIN_DRY: Stepper_toPosition(1); break;   // dry = +120°
    //case BIN_METAL: Stepper_toPosition(2); break; // metal = +240°
  //}
  //delay(500);

  // After rotation, check IR inside divider bin for object presence
  if (IR_detectObject()) {

    Buzzer_beep(1, 300);  // 1 beep of 300ms; you can adjust

      // rotate stepper to the correct position:
  switch (type) {
    case BIN_WET: Stepper_toPosition(0); break;   // wet = 0°
    case BIN_DRY: Stepper_toPosition(1); break;   // dry = +120°
    case BIN_METAL: Stepper_toPosition(2); break; // metal = +240°
  }
  delay(500);

    // object present -> open lid, wait 1000ms, close lid
    Servo_openLid();
    delay(1000);
    Servo_closeLid();
    delay(200);

    // Update depth reading for that bin (Ultrasonic is outside bottom lid)
    // move ultrasonic check for the currently selected down container:
    // (Ultrasonic_readCM reads current position below ultrasonic)
    float newDepth = Ultrasonic_readCM();
    if (newDepth < 999.0) binDepth[type] = newDepth;

    // Update fullness status for this bin
    binFull[type] = (binDepth[type] < BIN_FULL_DISTANCE);

    // Turn off LED for type
    LED_off(type + 1);

    // return stepper to home (0°)
    Stepper_toPosition(0);
    delay(200);

    // After successful drop, return to metal detection (loop will continue)
    return;
  } else {
    // No object detected in divider when expected: beep pattern 2 beeps 3 times with 500ms between pairs
    for (int r = 0; r < 3; r++) {
      //Buzzer_beep(2, 300);
      delay(500);
    }
    // Keep LED off for this type and return to metal detection
    LED_off(type + 1);
    // Optionally reset stepper home
    Stepper_toPosition(0);
    return;
  }
}

/////////////////////////
// Send SMS
/////////////////////////
void sendBinFullSMS(int type) {
  const char *binName;
  switch (type) {
    case BIN_WET: binName = "Wet bin"; break;
    case BIN_DRY: binName = "Dry bin"; break;
    case BIN_METAL: binName = "Metal bin"; break;
    default: binName = "Unknown bin"; break;
  }

  char message[64];
  snprintf(message, sizeof(message), "SmartBin Alert: %s is FULL and needs emptying now!", binName);

  Serial.print(F("[SMS] Sending: "));
  Serial.println(message);
  SIM800L_sendSMS(PHONE_NUMBER, message);
}
