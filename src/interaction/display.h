#ifndef _MOWERDISPLAY_H_
#define _MOWERDISPLAY_H_

#include <Arduino.h>
#include <ssd1306.h>
#include "mowermodel.h"
#include "interaction/presenter.h"
#include "logevent.h"
#include "utils.h"


class MOWERDISPLAY : public PRESENTER { 
    public:
        MOWERDISPLAY();
        void setup();
        void PresentMowerModel(MowerModel* model, bool forceFullPresentation);
        void SendLog(LogEvent *e);
        void setBlocked(bool blocked_);
    private:
        void print(String msg, int row);
        int rowHeight = 9;
        unsigned long messageDisplayedAt = 0;
        MowerModel *printedModel;
        String msg = "";
        bool _cleared = false;
        bool blocked = false;

};

#endif