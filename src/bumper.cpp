#include "bumper.h"
#include "Arduino.h"

BUMPER::BUMPER(int bumperPin_){
    bumperPin = bumperPin_;
}

void BUMPER::setup() {
    pinMode(bumperPin, INPUT_PULLUP);
}


void BUMPER::doLoop() {
    if (!IsBumped()) {
        lastUnbumped = millis();
    }
}

bool BUMPER::IsStuck() {
    return hasTimeout(lastUnbumped, 10000);
}


bool BUMPER::IsBumped() {
    return digitalRead(bumperPin) == LOW;
}