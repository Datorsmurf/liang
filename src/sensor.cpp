#include "sensor.h"
#include "Arduino.h"
#include "definitions.h"

#define INSIDE    1
#define NOSIGNAL  0
#define OUTSIDE   -1

#define SIGNAL_VALIDITY_TIME_MS 300
#define PULSE_UNIT_LENGTH 100


int SENSOR::outside_code[] = {OUTSIDE_BWF, INSIDE_BWF - OUTSIDE_BWF, OUTSIDE_BWF, INSIDE_BWF - OUTSIDE_BWF };
int SENSOR::inside_code[] = {INSIDE_BWF, INSIDE_BWF};

SENSOR::SENSOR(int pin_, bool missingSignalIsOut_){
    pin = pin_;
    missingSignalIsOut = missingSignalIsOut_;
    pulseHistoryPos = 0;
}

void SENSOR::setup() {
}

void SENSOR::handleInterrupt() {
  unsigned long now = micros();
  

  // Calculate the time since last pulse
  int time_since_pulse = int(now - last_pulse);
  last_pulse = now;

  // Convert to pulse units (rounding up)
  int pulse_length = (time_since_pulse+(PULSE_UNIT_LENGTH/2)) / PULSE_UNIT_LENGTH;

  // Check if the latest pulse fits the code for inside
  if (abs(pulse_length-inside_code[pulse_count_inside]) < 2) {
    pulse_count_inside++;

    // Check if the entire pulse train has been batched
    if (pulse_count_inside >= sizeof(inside_code)/sizeof(inside_code[0])) {
      lastInTime = now;
      pulse_count_inside=0;
    }
  } else {
    pulse_count_inside=0;
  }

  // Check if the latest pulse fits the code for outside
  if (abs(pulse_length-outside_code[pulse_count_outside]) < 2) {
    pulse_count_outside++;
    if (pulse_count_outside >= sizeof(outside_code)/sizeof(outside_code[0])) {
      lastOutTime = now;
      pulse_count_outside=0;
    }
  } else {
    pulse_count_outside=0;

  }
  
  pulseHistoryPos = pulseHistoryPos++ % PULSE_HISTORY_COUNT;
  pulsehistory[pulseHistoryPos] = pulse_length;

  // Store the received code for debug output
  // arr[arr_count++] = pulse_length;
  // if (arr_count>arr_length) arr_count=0;
}

bool SENSOR::IsIn() {
    return lastInTime > lastOutTime;
}

bool SENSOR::IsOut() {
    return lastOutTime > lastInTime;
}

bool SENSOR::IsOutOfBounds() {
    return missingSignalIsOut ? !IsIn() : IsOut();
}