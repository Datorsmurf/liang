#include "bumper.h"
#include "Arduino.h"

BUMPER::BUMPER(int bumperPin_){
    bumperPin = bumperPin_;
}

void BUMPER::setup() {
    pinMode(bumperPin, INPUT);
}

bool BUMPER::IsBumped() {
    return digitalRead(bumperPin) == HIGH;
}