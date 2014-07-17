#ifndef HWLAYERMOCK_H
#define HWLAYERMOCK_H

#include "../../hw/IHWLayer.h"

class HWLayerMock : public IHWLayer
{
public:
    HWLayerMock();    

    virtual IHWLayer::ButtonState getButtonState(unsigned char index);
    virtual void setLED(unsigned char index, IHWLayer::LedState state);
    virtual unsigned char getKnobValue(unsigned char index);
    virtual unsigned char readSRAM(long address) {return 0;};
    virtual void readSRAM(long address, unsigned char * data, unsigned int size) {};
    virtual void writeSRAM(long address, unsigned char data) {};
    virtual void writeSRAM(long address, unsigned char * data, unsigned int size) {};
    virtual unsigned int getElapsedBastlCycles() {return 0;};
    virtual unsigned char getBastlCyclesPerSecond() {return 0;};


    void setButtonState(unsigned char buttonIndex, bool newState);
private:
    bool buttonStates_[16];

};

#endif // HWLAYERMOCK_H
