#include <stdio.h>
#include <inttypes.h>


#include "../mapping.h"





int main( int argc, const char* argv[] ) {

for (uint16_t index=0; index<256; index++) {
	printf("%u ",index);

	// unsigned
	printf("%u ",mapLinearRuntimeFastU8U16((uint8_t)index,0,-1));
	printf("%u ",mapLinearRuntimePreciseU8U16((uint8_t)index,0,-1));
	printf("%u ",mapLinearRuntimeFastU8U16((uint8_t)index,-1,0));
	printf("%u ",mapLinearRuntimePreciseU8U16((uint8_t)index,-1,0));

	// signed
	printf("%i ",mapLinearRuntimeFastU8S16((uint8_t)index,-30000,30000));
	printf("%i ",mapLinearRuntimePreciseU8S16((uint8_t)index,-30000,30000));
	printf("%i ",mapLinearRuntimeFastU8S16((uint8_t)index,30000,-30000));
	printf("%i ",mapLinearRuntimePreciseU8S16((uint8_t)index,30000,-30000));
	printf("\n");
}


}
