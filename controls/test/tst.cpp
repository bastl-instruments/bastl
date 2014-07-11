
#include "HWLayerMock.h"
#include "../RadioButtons.h"
#include <iostream>

int main( int argc, const char* argv[] ) {
    HWLayerMock hw;
    unsigned char radioButtons[16] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30};
    RadioButtons buttons(&hw, &(radioButtons[0]), 16);
    unsigned char selectedButton = 0;

    printf("\tTest 1 - Default radio state \t\t%s\n", !buttons.getSelectedButton(selectedButton) ? "OK" : "Error");

    hw.setButtonState(2, true);
    buttons.update();
    printf("\tTest 2 - Radio State after select \t\t%s\n", buttons.getSelectedButton(selectedButton) && selectedButton == 2 ? "OK" : "Error");

    hw.setButtonState(2, false);
    hw.setButtonState(8, true);
    buttons.update();
    printf("\tTest 3 - Radio state after select another \t\t%s\n", buttons.getSelectedButton(selectedButton) && selectedButton == 8 ? "OK" : "Error");

    buttons.update();
    printf("\tTest 3 - Radio state after select the same \t\t%s\n", !buttons.getSelectedButton(selectedButton) ? "OK" : "Error");
}
