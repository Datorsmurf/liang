#ifndef _BUMPER_H_
#define _BUMPER_H_


class BUMPER { 
    public:
        BUMPER(int bumperPin_);
        void setup();
        bool IsBumped();
    private:
        int bumperPin;

};

#endif