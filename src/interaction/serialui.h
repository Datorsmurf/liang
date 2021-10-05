#ifndef _SERIALUI_H_
#define _SERIALUI_H_

#include "Logger.h"
#include "interaction/presenter.h"
#include "logevent.h"

#include "definitions.h"


class SERIALUI : public PRESENTER { 
    public:

        SERIALUI(ModeSelectEvent modeSelectEvent_);
        void setup();
        void doLoop();
        void PresentMowerModel(MowerModel* model, bool forceFullPresentation);
        void SendLog(LogEvent *e);


    private:
        ModeSelectEvent modeSelectEvent;
};


#endif