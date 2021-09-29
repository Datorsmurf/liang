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
  //Serial.println("B1 " + String(readValue));
  readValue = floatMap(readValue, 0, ANALOG_RESOLUTION_MAX_VALUE, 310, 3070);
//  readValue = floatMap(readValue, 0, 3153, 306, ANALOG_RESOLUTION_MAX_VALUE);
  //Serial.println("B4 " + String(readValue));

  //Serial.println("r1 " + String(ANALOG_RESOLUTION_MAX_VALUE));

  //Convert to volts on the pin
  float reading = readValue / (3153/3.3);
  //Serial.println("B2 " + String(reading, 3));
  //Serial.println();
  // Adjust for voltage divider circuit
  reading = (reading * VOLTDIVATOR);
  //Serial.println("B3 " + String(reading, 3));
  return reading;

}
