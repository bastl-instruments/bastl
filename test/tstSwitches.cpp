
#include "HWLayerMock.h"
#include "../RadioButtons.h"
#include "../Switches.h"
#include <cstdio>

int main( int argc, const char* argv[] ) {
    HWLayerMock hw;
    printf("Test Switches (down activated)\n");

    unsigned char switchesButtons[16] = {0, 2, 4/*first used button*/, 6, 8, 10, 12, 14, 16};
    Switches switches;
    switches.init(&hw, &switchesButtons[2], 6);

    hw.setButtonState(8, true);
    switches.update();
    printf("\tTest 7 - Button pressed \t\t%s\n", switches.getStatus(2) ? "OK" : "Error");
    switches.update();
    printf("\tTest 8 - Button keep pressed \t\t%s\n", switches.getStatus(2) ? "OK" : "Error");
    hw.setButtonState(8, false);
    switches.update();
    printf("\tTest 9 - Button released \t\t%s\n", switches.getStatus(2) ? "OK" : "Error");
    hw.setButtonState(8, true);
    switches.update();
    printf("\tTest 10 - Button pressed again \t\t%s\n", !switches.getStatus(2) ? "OK" : "Error");

    switches.setStatus(3, true);
    printf("\tTest 11 - Button set externally to pressed \t\t%s\n", switches.getStatus(3) ? "OK" : "Error");
    switches.setStatus(3, false);
    printf("\tTest 12 - Button set externally to released \t\t%s\n", !switches.getStatus(3) ? "OK" : "Error");

    printf("Test Switches (up activated)\n");

    Switches switches2;
    switches2.init(&hw, &switchesButtons[2], 6, IButtonHW::UP);

    hw.setButtonState(12, true);
    switches2.update();
    printf("\tTest 7 - Button pressed \t\t%s\n", !switches2.getStatus(4) ? "OK" : "Error");
    switches2.update();
    printf("\tTest 8 - Button keep pressed \t\t%s\n", !switches2.getStatus(4) ? "OK" : "Error");
    hw.setButtonState(12, false);
    switches2.update();
    printf("\tTest 9 - Button released \t\t%s\n", switches2.getStatus(4) ? "OK" : "Error");
    hw.setButtonState(12, true);
    switches2.update();
    printf("\tTest 10 - Button pressed again \t\t%s\n", switches2.getStatus(4) ? "OK" : "Error");
    hw.setButtonState(12, false);
    switches2.update();
    printf("\tTest 10 - Button pressed again \t\t%s\n", !switches2.getStatus(4) ? "OK" : "Error");

    switches2.setStatus(3, true);
    printf("\tTest 11 - Button set externally to pressed \t\t%s\n", switches2.getStatus(3) ? "OK" : "Error");
    switches2.setStatus(3, false);
    printf("\tTest 12 - Button set externally to released \t\t%s\n", !switches2.getStatus(3) ? "OK" : "Error");
}
