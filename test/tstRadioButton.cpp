
#include "HWLayerMock.h"
#include "../RadioButtons.h"
//#include "../Switches.h"
#include <cstdio>

int main( int argc, const char* argv[] ) {
    HWLayerMock hw;
    unsigned char radioButtons[16] = {0, 2, 4, 6, 8/*first in test*/, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30};
    unsigned char * button = &(radioButtons[4]);
    RadioButtons buttons(&hw, button, 6);
    unsigned char selectedButton = 0;

    printf("Radio Buttons\n");
    printf("\tTest 1 - Default radio state \t\t%s\n", !buttons.getSelectedButton(selectedButton) ? "OK" : "Error");

    hw.setButtonState(10, true);
    buttons.update();
    printf("\tTest 2 - Radio State after select \t\t%s\n", buttons.getSelectedButton(selectedButton) && selectedButton == 1 ? "OK" : "Error");

    hw.setButtonState(10, false);
    hw.setButtonState(14, true);
    buttons.update();
    printf("\tTest 3 - Radio state after select another \t\t%s\n", buttons.getSelectedButton(selectedButton) && selectedButton == 3 ? "OK" : "Error");

    buttons.update();
    printf("\tTest 4 - Not released button \t\t%s\n", buttons.getSelectedButton(selectedButton) && selectedButton == 3 ? "OK" : "Error");

    hw.setButtonState(14, false);
    buttons.update();
    printf("\tTest 5 - Released button \t\t%s\n", buttons.getSelectedButton(selectedButton) && selectedButton == 3 ? "OK" : "Error");

    hw.setButtonState(14, true);
    buttons.update();
    printf("\tTest 6 - Button pressed again \t\t%s\n", !buttons.getSelectedButton(selectedButton) ? "OK" : "Error");

    hw.setButtonState(14, false);
    buttons.update();
    hw.setButtonState(14, true);
    buttons.update();
    printf("\tTest 7 - Button release and pressed again \t\t%s\n", buttons.getSelectedButton(selectedButton) && selectedButton == 3 ? "OK" : "Error");


    /*printf("Down activate Switches\n");

    unsigned char switchesButtons[16] = {0, 2, 4, 6, 8, 10, 12, 14, 16};
    Switches switches;
    switches.init(&hw, switchesButtons, 9);

    hw.setButtonState(16, true);
    switches.update();
    printf("\tTest 7 - Button pressed \t\t%s\n", switches.getStatus(8) ? "OK" : "Error");
    switches.update();
    printf("\tTest 8 - Button keep pressed \t\t%s\n", switches.getStatus(8) ? "OK" : "Error");
    hw.setButtonState(16, false);
    switches.update();
    printf("\tTest 9 - Button released \t\t%s\n", switches.getStatus(8) ? "OK" : "Error");
    hw.setButtonState(16, true);
    switches.update();
    printf("\tTest 10 - Button pressed again \t\t%s\n", !switches.getStatus(8) ? "OK" : "Error");

    switches.setStatus(8, true);
    printf("\tTest 11 - Button set externally to pressed \t\t%s\n", switches.getStatus(8) ? "OK" : "Error");
    switches.setStatus(8, false);
    printf("\tTest 12 - Button set externally to released \t\t%s\n", !switches.getStatus(8) ? "OK" : "Error");

    printf("Up activate Switches\n");

    Switches switches2;
    switches2.init(&hw, switchesButtons, 9, IHWLayer::UP);

    hw.setButtonState(16, true);
    switches2.update();
    printf("\tTest 7 - Button pressed \t\t%s\n", !switches2.getStatus(8) ? "OK" : "Error");
    switches2.update();
    printf("\tTest 8 - Button keep pressed \t\t%s\n", !switches2.getStatus(8) ? "OK" : "Error");
    hw.setButtonState(16, false);
    switches2.update();
    printf("\tTest 9 - Button released \t\t%s\n", switches2.getStatus(8) ? "OK" : "Error");
    hw.setButtonState(16, true);
    switches2.update();
    printf("\tTest 10 - Button pressed again \t\t%s\n", switches2.getStatus(8) ? "OK" : "Error");
    hw.setButtonState(16, false);
    switches2.update();
    printf("\tTest 10 - Button pressed again \t\t%s\n", !switches2.getStatus(8) ? "OK" : "Error");

    switches2.setStatus(8, true);
    printf("\tTest 11 - Button set externally to pressed \t\t%s\n", switches2.getStatus(8) ? "OK" : "Error");
    switches2.setStatus(8, false);
    printf("\tTest 12 - Button set externally to released \t\t%s\n", !switches2.getStatus(8) ? "OK" : "Error");*/







}
