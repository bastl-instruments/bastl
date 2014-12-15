#ifndef HWLAYERMOCK_H
#define HWLAYERMOCK_H

#include "../IButtonHW.h"

class HWLayerMock : public IButtonHW
{
public:
    HWLayerMock();    

    virtual IButtonHW::ButtonState getButtonState(unsigned char index);
    void setButtonState(unsigned char buttonIndex, bool newState);
private:
    bool buttonStates_[32];

};

#endif // HWLAYERMOCK_H
