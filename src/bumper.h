#ifndef _BUMPER_H_
#define _BUMPER_H_

#include "utils.h"

class BUMPER { 
    public:
        BUMPER(int bumperPin_);
        void setup();
        void doLoop();
        bool IsBumped();
        bool IsStuck();
    private:
        int bumperPin;
        unsigned long lastUnbumped;

};

#endif