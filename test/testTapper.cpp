#include <stdio.h>
#include <inttypes.h>


#include "../Tapper.h"


Tapper tapper;



int main( int argc, const char* argv[] ) {

	tapper.init(200,10);
	tapper.tap(0);
	tapper.tap(100);
	tapper.tap(220);
	tapper.tap(310);

	tapper.tap(500);
	tapper.tap(550);


	printf("speed: %u\n",tapper.getTimeUnitsPerStep());

}
