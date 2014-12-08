#include <stdio.h>
#include <inttypes.h>
#include "../lfoExtended.h"

lfoExtended<1000> lfo;



int main( int argc, const char* argv[] ) {

	lfo.init();
	lfo.setWaveform(RANDOM);
	lfo.setFreq(100<<4);


	for (uint16_t index=0; index < 300; index++) {
		lfo.step();
		//lfo.getCurrentValue();
		printf("%u\n",lfo.getCurrentValue());
	}


}
