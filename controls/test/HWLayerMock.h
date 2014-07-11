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

private:
    bool buttonStates_[16];

};

#endif // HWLAYERMOCK_H
