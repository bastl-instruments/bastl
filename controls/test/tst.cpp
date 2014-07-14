
#include "HWLayerMock.h"
#include "../RadioButtons.h"
#include "../Switches.h"
#include <iostream>

int main( int argc, const char* argv[] ) {
    HWLayerMock hw;
    unsigned char radioButtons[16] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30};
    RadioButtons buttons(&hw, &(radioButtons[0]), 16);
    unsigned char selectedButton = 0;

    printf("Radio Buttons\n");
    printf("\tTest 1 - Default radio state \t\t%s\n", !buttons.getSelectedButton(selectedButton) ? "OK" : "Error");

    hw.setButtonState(2, true);
    buttons.update();
    printf("\tTest 2 - Radio State after select \t\t%s\n", buttons.getSelectedButton(selectedButton) && selectedButton == 1 ? "OK" : "Error");

    hw.setButtonState(2, false);
    hw.setButtonState(8, true);
    buttons.update();
    printf("\tTest 3 - Radio state after select another \t\t%s\n", buttons.getSelectedButton(selectedButton) && selectedButton == 4 ? "OK" : "Error");

    buttons.update();
    printf("\tTest 4 - Not released button \t\t%s\n", buttons.getSelectedButton(selectedButton) && selectedButton == 4 ? "OK" : "Error");

    hw.setButtonState(8, false);
    buttons.update();
    printf("\tTest 5 - Released button \t\t%s\n", buttons.getSelectedButton(selectedButton) && selectedButton == 4 ? "OK" : "Error");

    hw.setButtonState(8, true);
    buttons.update();
    printf("\tTest 6 - Button pressed again \t\t%s\n", !buttons.getSelectedButton(selectedButton) ? "OK" : "Error");

    hw.setButtonState(8, false);

    printf("Switches\n");

    unsigned char switchesButtons[16] = {0, 2, 4, 6, 8};
    Switches switches(&hw, &(switchesButtons[0]), 5);

    hw.setButtonState(2, true);
    switches.update();
    printf("\tTest 7 - Button pressed \t\t%s\n", switches.getStatus(1) ? "OK" : "Error");
    switches.update();
    printf("\tTest 8 - Button keep pressed \t\t%s\n", switches.getStatus(1) ? "OK" : "Error");
    hw.setButtonState(2, false);
    switches.update();
    printf("\tTest 9 - Button released \t\t%s\n", switches.getStatus(1) ? "OK" : "Error");
    hw.setButtonState(2, true);
    switches.update();
    printf("\tTest 10 - Button pressed again \t\t%s\n", !switches.getStatus(1) ? "OK" : "Error");

    switches.setStatus(1, true);
    printf("\tTest 11 - Button set externally to pressed \t\t%s\n", switches.getStatus(1) ? "OK" : "Error");
    switches.setStatus(1, false);
    printf("\tTest 12 - Button set externally to released \t\t%s\n", !switches.getStatus(1) ? "OK" : "Error");





}
