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
#define BATTERY_FULL_MV       12560
#define BATTERY_EMPTY_MV      10400

//NiMH
//#define BATTERY_FULL_MV        14500
//#define BATTERY_EMPTY_MV       11500

//LEAD ACID
//#define BATTERY_FULL_MV    13300
//#define BATTERY_EMPTY_MV   12000

// Running average sample size
#define FILTER        200

// Voltage divider factor x10 to avoid decimal ( 4 = 40)
#define VOLTDIVATOR   42


class BATTERY {
  public:
    BATTERY(int type, int sensepin, int dockpin);

    int getBatteryType();

    void setFullyChargedLevel(int level);
    int getFullyChargedLevel();

    void setDepletedLevel(int level);
    int getDepletedLevel();

    bool mustCharge();
    bool isBeingCharged();
    bool isFullyCharged();

    int getVoltage();
    void resetVoltage();
    void updateVoltage();
    unsigned long readBatteryAndCalcValue();

  private:
    int batType;
    int batSensePin;
    int batDockPin;
    int fullyChargedLevel = BATTERY_FULL_MV;
    int depletedLevel = BATTERY_EMPTY_MV;
    float averageVoltage;
};

#endif /* _BATTERY_H_ */
