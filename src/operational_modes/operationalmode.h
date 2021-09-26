#ifndef _OPERATIONALMODE_H_
#define _OPERATIONALMODE_H_

#define OP_MODE_IDLE 0
#define OP_MODE_CHARGE 1
#define OP_MODE_MOW 2
#define OP_MODE_MOW_ONCE 3
#define OP_MODE_UPGRADE 4

#include "behaviors/behavior.h"
#include "Arduino.h"


class OPERATIONALMODE { 
    public:
    //Do what's needed for the mode and return the Id for the bahavior to start off with
    virtual int start();

    virtual int loop();

    //The... trumroll... Id
    virtual int id();

    virtual String desc();
};

#endif