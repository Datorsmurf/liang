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
#define BATTERY_FULL       12.560
#define BATTERY_EMPTY      10.400

// Running average sample size
#define FILTER        1000

#define VOLTDIVATOR   4.33

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
