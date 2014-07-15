#ifndef SWITCHES_H
#define SWITCHES_H

#define GETBIT(X, Y)  (((X) & (1 << (Y))) >> (Y) == 1)
#define SETBITTRUE(X, Y) ((X) = ((X) | (1 << (Y))))
#define SETBITFALSE(X, Y) ((X) = ((X) & ~(1 << (Y))))


#include "../hw/IHWLayer.h"

//This class hadnles maximum 8 independent swithces
class Switches
{
public:
    Switches(IHWLayer *hwLayer, unsigned char * buttonIndexes, unsigned char count);
    bool getStatus(unsigned char buttonIndex);
    void setStatus(unsigned char buttonIndex, bool value);
    void update();
private:
    IHWLayer * hwLayer_;
    unsigned char * buttonIndexes_;
    unsigned char buttonCount_;
    unsigned char lastStates_;
    unsigned char changedStates_;
    unsigned char statuses_;
};

#endif // SWITCHES_H
