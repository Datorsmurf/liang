/*
 Liam - DIY Robot Lawn Mower

 Battery Library

 ======================
  Licensed under GPLv3
 ======================
*/

#include "Arduino.h"
#include "battery.h"
#include "definitions.h"

BATTERY::BATTERY(int sensepin, int dockpin) {
  batSensePin = sensepin;
  batDockPin = dockpin;

  fullyChargedLevel = BATTERY_FULL;
  depletedLevel = BATTERY_EMPTY;
  resetVoltage();
}

bool BATTERY::mustCharge() {
  return (averageVoltage < depletedLevel);
}

bool BATTERY::isBeingCharged() {
  return digitalRead(batDockPin);
}

bool BATTERY::isFullyCharged() {
  
  return averageVoltage > fullyChargedLevel;
}


// Get battery voltage in mV (filtered through running average)
int BATTERY::getVoltage() {
  return averageVoltage;
}

void BATTERY::resetVoltage() {
  averageVoltage = readBatteryAndCalcValue();
}

void BATTERY::setup() {
  pinMode(batDockPin, INPUT_PULLDOWN);
  pinMode(batSensePin, ANALOG);
  adcAttachPin(batSensePin);

  resetVoltage();
}


// Take a battery reading and recalculate running average
float BATTERY::updateVoltage() {
  averageVoltage -= averageVoltage / FILTER;
  averageVoltage += readBatteryAndCalcValue() / FILTER;
  return averageVoltage;
}


float BATTERY::readBatteryAndCalcValue(){
  int readValue = analogRead(batSensePin);
  #if BOARD_VERSION == 10
    float reading = 1.1 +  (readValue * (13.3 / ANALOG_RESOLUTION_MAX_VALUE));
  #elif BOARD_VERSION == 11
    // float reading = 0.85 +  (readValue * (16.5 / ANALOG_RESOLUTION_MAX_VALUE));
    float reading = (R_IN+R_GND)/R_GND * readValue * (3.3/ANALOG_RESOLUTION_MAX_VALUE);
  #endif
  return reading;
}
