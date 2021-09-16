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

  fullyChargedLevel = BATTERY_FULL_MV;
  depletedLevel = BATTERY_EMPTY_MV;
}

bool BATTERY::mustCharge() {
  return (averageVoltage < depletedLevel);
}

bool BATTERY::isBeingCharged() {
  return digitalRead(batDockPin);
}

bool BATTERY::isFullyCharged() {
  return (readBatteryAndCalcValue() > fullyChargedLevel);
}


// Get battery voltage in mV (filtered through running average)
int BATTERY::getVoltage() {
  return averageVoltage;
}

void BATTERY::resetVoltage() {
  averageVoltage = readBatteryAndCalcValue();
}

// Take a battery reading and recalculate running average
void BATTERY::updateVoltage() {
  averageVoltage -= averageVoltage / FILTER;
  averageVoltage += readBatteryAndCalcValue() / FILTER;
}

// Measure battery voltage in mV
unsigned long BATTERY::readBatteryAndCalcValue(){
  unsigned long reading = analogRead(batSensePin);

  // Convert from ADC units to uV
  reading = reading * 4880;
  // Adjust for voltage divider circuit
  reading = (reading * VOLTDIVATOR) / 10;

  // Convert to mV
   return reading / 1000;

}