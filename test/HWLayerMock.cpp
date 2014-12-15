#include "HWLayerMock.h"

HWLayerMock::HWLayerMock()
{
    for (unsigned char i = 0; i < 32; i++) {
        buttonStates_[i] = false;
    }
}

IButtonHW::ButtonState HWLayerMock::getButtonState(unsigned char index)
{
    return buttonStates_[index] ? IButtonHW::DOWN : IButtonHW::UP ;
}

void HWLayerMock::setButtonState(unsigned char buttonIndex, bool newState)
{
    buttonStates_[buttonIndex] = newState;
}
