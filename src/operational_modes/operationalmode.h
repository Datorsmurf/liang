#ifndef _OPERATIONALMODE_H_
#define _OPERATIONALMODE_H_

#define OP_MODE_IDLE 0
#define OP_MODE_CHARGE 1
#define OP_MODE_MOW 2
#define OP_MODE_MOW_ONCE 3

#include "behaviors/behavior.h"


class OPERATIONALMODE { 
    public:
    virtual int start();
    virtual int loop();
    virtual int id();
};

#endif