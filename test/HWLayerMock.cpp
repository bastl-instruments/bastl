#include "HWLayerMock.h"

HWLayerMock::HWLayerMock()
{
    for (unsigned char i = 0; i < 16; i++) {
        buttonStates_[i] = false;
    }
}

IButtonHW::ButtonState HWLayerMock::getButtonState(unsigned char index)
{
    return buttonStates_[index / 2] ? IButtonHW::DOWN : IButtonHW::UP ;
}

void HWLayerMock::setButtonState(unsigned char buttonIndex, bool newState)
{
    buttonStates_[buttonIndex / 2] = newState;
}
