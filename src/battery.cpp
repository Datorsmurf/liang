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
  pinMode(batSensePin, INPUT);
  adcAttachPin(batSensePin);

  fullyChargedLevel = BATTERY_FULL;
  depletedLevel = BATTERY_EMPTY;
  resetVoltage();
}

bool BATTERY::mustCharge() {
  return (averageVoltage < depletedLevel);
}

bool BATTERY::isBeingCharged() {
  return true;
  //return digitalRead(batDockPin);
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
  resetVoltage();
}


// Take a battery reading and recalculate running average
float BATTERY::updateVoltage() {
  averageVoltage -= averageVoltage / FILTER;
  averageVoltage += readBatteryAndCalcValue() / FILTER;
  return averageVoltage;
}


float BATTERY::readBatteryAndCalcValue(){
  float reading = analogRead(batSensePin);

  //Convert to volts on the pin
  reading = reading / ANALOG_RESOLUTION_MAX_VALUE*3.3;
  
  // Adjust for voltage divider circuit
  reading = (reading * VOLTDIVATOR);
  return reading;

}
