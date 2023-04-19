/*
 Liam - DIY Robot Lawn Mower

 Battery Library

 ======================
  Licensed under GPLv3
 ======================
*/


#ifndef _BATTERY_H_
#define _BATTERY_H_

#include "utils.h"

// Voltages should be specified in V
#define BATTERY_FULL       15.000 //TODO: Change when changing to new battery!
#define BATTERY_EMPTY      13.600 //TODO: Change when changing to new battery!

// Running average sample size
#define FILTER        2000

// Voltage divider resistors (Same for both battery and charge connections)
#define R_IN    330000  // Resistance in ohm of resistor connected between battery+ and analog input pin.
#define R_GND   82000   // Resistance in ohm of resistor connected between GND and analog input pin.

class BATTERY {
  public:
    BATTERY(int sensepin, int dockpin);

    bool mustCharge();
    bool isBeingCharged();
    bool isFullyCharged();

    int getVoltage();
    void resetVoltage();
    void setup();

    float updateVoltage();

  private:
    float readBatteryAndCalcValue();
    int batSensePin;
    int batDockPin;
    int fullyChargedLevel;
    int depletedLevel;
    float averageVoltage;
};

#endif /* _BATTERY_H_ */
