#ifndef _BEHAVIOR_H_
#define _BEHAVIOR_H_


#define BEHAVIOR_IDLE 0
#define BEHAVIOR_MOW 1
#define BEHAVIOR_LOOK_FOR_BWF 2
#define BEHAVIOR_FOLLOW_BWF 3
#define BEHAVIOR_GO_AROUND_OBSTICLE 4
#define BEHAVIOR_CHARGE 5
#define BEHAVIOR_LAUNCH 6

class BEHAVIOR { 
    public:
    virtual void start();
    virtual int loop();
    virtual int id();
};

#endif