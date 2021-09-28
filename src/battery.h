/*
 Liam - DIY Robot Lawn Mower

 Battery Library

 ======================
  Licensed under GPLv3
 ======================
*/


#ifndef _BATTERY_H_
#define _BATTERY_H_

// Voltages should be specified in mV (12.56 V = 12560 mV)

//LION
#define BATTERY_FULL       12.560
#define BATTERY_EMPTY      10.400

//NiMH
//#define BATTERY_FULL_MV        14500
//#define BATTERY_EMPTY_MV       11500

//LEAD ACID
//#define BATTERY_FULL_MV    13300
//#define BATTERY_EMPTY_MV   12000

// Running average sample size
#define FILTER        200

#define VOLTDIVATOR   3.63


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
