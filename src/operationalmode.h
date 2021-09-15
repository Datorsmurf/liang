#ifndef _OPERATIONALMODE_H_
#define _OPERATIONALMODE_H_


class OPERATIONALMODE { 
    public:
    virtual void start();
    virtual int loop();
    virtual int id();
};

#endif