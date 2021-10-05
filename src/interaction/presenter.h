#ifndef _PRESENTER_H_
#define _PRESENTER_H_

#include "mowermodel.h"
#include "logevent.h"

class PRESENTER { 
    public:
        virtual void PresentMowerModel(MowerModel* model, bool forceFullPresentation) = 0;
        virtual void SendLog(LogEvent *e) = 0;
};

#endif