#ifndef _MOWERDISPLAY_H_
#define _MOWERDISPLAY_H_

#include <Arduino.h>
#include <ssd1306.h>
#include "mowermodel.h"
//#include "mutex"


class MOWERDISPLAY { 
    public:
        MOWERDISPLAY();
        void setup();
        void DrawMowerModel(MowerModel* model);
        void setBlocked(bool blocked_);
    private:
        void print(String msg, int row);
        int rowHeight = 9;
        unsigned long messageDisplayedAt = 0;
        MowerModel *printedModel;
        bool _cleared = false;
        bool blocked = false;

};

#endif