#ifndef HWLAYERMOCK_H
#define HWLAYERMOCK_H

#include "../../hw/IHWLayer.h"

class HWLayerMock : public IHWLayer
{
public:
    HWLayerMock();    

    virtual bool buttonDown(unsigned char index);
    virtual void setDiod(unsigned char diod);
    virtual unsigned char getKnobValue(unsigned char index);
    void setButtonState(unsigned char buttonIndex, bool newState);
    virtual unsigned char readSRAM(long address) {return 0;};
    virtual void readSRAMMore(long address, unsigned char * data, unsigned char size) {};
    virtual void writeSRAM(long address, unsigned char data) {};
    virtual void writeSRAMMore(long address, unsigned char * data, unsigned char size) {};

private:
    bool buttonStates_[16];

};

#endif // HWLAYERMOCK_H
