#include "HWLayerMock.h"

HWLayerMock::HWLayerMock()
{
    for (unsigned char i = 0; i < 16; i++) {
        buttonStates_[i] = false;
    }
}

IHWLayer::ButtonState HWLayerMock::getButtonState(unsigned char index)
{
    return buttonStates_[index / 2] ? IHWLayer::DOWN : IHWLayer::UP ;
}

void HWLayerMock::setLED(unsigned char diod)
{

}

unsigned char HWLayerMock::getKnobValue(unsigned char index)
{
    return 0;
}

void HWLayerMock::setButtonState(unsigned char buttonIndex, bool newState)
{
    buttonStates_[buttonIndex / 2] = newState;
}
