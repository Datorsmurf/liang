#include "utils.h"

float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
    const float dividend = out_max - out_min;
    const float divisor = in_max - in_min;
    const float delta = x - in_min;
    if(divisor == 0){
        //log_e("Invalid map input range, min == max");
        return -1; //AVR returns -1, SAM returns 0
    }
    return (delta * dividend + (divisor / 2)) / divisor + out_min;
}


int absDiff(int i1, int i2) {
    if (i1 > i2) return i1-i2;
    return i2-i1;
}
