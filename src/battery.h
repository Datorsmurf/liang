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
#define BATTERY_FULL       12.560 //TODO: Change according to actual battery used!
#define BATTERY_EMPTY      10.400 //TODO: Change according to actual battery used!

// Running average sample size
#define FILTER        2000

// Uncomment below to choose between theoretical values for the voltage divider or emperical values (by measureing the input voltage and between R36 & R50):
// #define VOLTDIV_THEO
#define VOLTDIV_EMPE

#ifdef VOLTDIV_THEO
  // Voltage divider resistors (Same for both battery and charge connections)
  #define R_IN    330000  // Resistance in ohm of resistor connected between battery+ and analog input pin.
  #define R_GND   82000   // Resistance in ohm of resistor connected between GND and analog input pin.
  #define VOLT_DIV ((R_IN+R_GND)/R_GND)
#endif

#ifdef VOLTDIV_EMPE
  #define MEASURED_INPUT_V  13.94
  #define MEASURED_DIV_V    2.750  // Voltage to GND from between R36 & R50
  #define VOLT_DIV (MEASURED_INPUT_V/MEASURED_DIV_V)
#endif

// ADC Calibrations (Used for rough compensation for the nonlinear and not factory calibrated adc in ESP32)
#define ADC_SATURATION_V 3.195    // Not used in calculations, but good to have for future reference
#define ADC_LINEAR_MAX_V 2.75     // Voltage on gpio when readings starts to become exponential
#define ADC_LINEAR_MAX_READING 1650 // ADC reading at ADC_LINEAR_MAX_V
#define ADC_DETECTION_V 0.21      // Online sources claim 0.15V, but by raising this value the slope of the linear approximation is increased, and the accuracy for both low and high input voltages is improved. 

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
