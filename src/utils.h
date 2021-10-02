#ifndef _MOWUTILS_H_
#define _MOWUTILS_H_

#include "Arduino.h"
float floatMap(float x, float in_min, float in_max, float out_min, float out_max);
int absDiff(int i1, int i2);
bool hasTimeout(unsigned long startTime, unsigned long period);



#endif