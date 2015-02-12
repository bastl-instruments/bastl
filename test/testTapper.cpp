#include <stdio.h>
#include <inttypes.h>


#include "../Tapper.h"


Tapper tapper;



int main( int argc, const char* argv[] ) {

	tapper.init(200,10);

	// Normal tapping with ~100
	tapper.tap(0);
	printf("--> speed: %u\n",tapper.getTimeUnitsPerStep());
	tapper.tap(100);
	printf("--> speed: %u\n",tapper.getTimeUnitsPerStep());
	tapper.tap(220);
	printf("--> speed: %u\n",tapper.getTimeUnitsPerStep());
	tapper.tap(310);
	printf("--> speed: %u\n",tapper.getTimeUnitsPerStep());

	// longer tap time difference to exit cycle and tapping at ~50
	tapper.tap(500);
	printf("--> speed: %u\n",tapper.getTimeUnitsPerStep());
	tapper.tap(550);
	printf("--> speed: %u\n",tapper.getTimeUnitsPerStep());
	tapper.tap(610);
	printf("--> speed: %u\n",tapper.getTimeUnitsPerStep());
	tapper.tap(655);
	printf("--> speed: %u\n",tapper.getTimeUnitsPerStep());

	// some single taps that indicate only hitting the first beat of bar with
	tapper.tap(800);
	printf("--> speed: %u\n",tapper.getTimeUnitsPerStep());
	tapper.tap(1100);
	printf("--> speed: %u\n",tapper.getTimeUnitsPerStep());
	tapper.tap(1500);
	printf("--> speed: %u\n",tapper.getTimeUnitsPerStep());

	printf("\nTestdata\n");
	tapper.init(2000*60.0/30,1,255);
	tapper.tap(49982);
	printf("--> speed: %u\n",tapper.getTimeUnitsPerStep());
	tapper.tap(50395);
	printf("--> speed: %u\n",tapper.getTimeUnitsPerStep());
	tapper.tap(50767);
	printf("--> speed: %u\n",tapper.getTimeUnitsPerStep());
	tapper.tap(51181);
	printf("--> speed: %u\n",tapper.getTimeUnitsPerStep());
	tapper.tap(21912);
	printf("--> speed: %u\n",tapper.getTimeUnitsPerStep());
	tapper.tap(24713);
	printf("--> speed: %u\n",tapper.getTimeUnitsPerStep());
	tapper.tap(27311);
	printf("--> speed: %u\n",tapper.getTimeUnitsPerStep());
	tapper.tap(30396);
	printf("--> speed: %u\n",tapper.getTimeUnitsPerStep());
	tapper.tap(33319);
	printf("--> speed: %u\n",tapper.getTimeUnitsPerStep());
	tapper.tap(36161);
	printf("--> speed: %u\n",tapper.getTimeUnitsPerStep());
	tapper.tap(38922);
	printf("--> speed: %u\n",tapper.getTimeUnitsPerStep());





}
