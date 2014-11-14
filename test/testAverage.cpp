#include <stdio.h>
#include <inttypes.h>


#include "../movingAverage.h"


MovingAverageLinear<uint8_t> average(4);



int main( int argc, const char* argv[] ) {



	printf("%u\n",average.getAverage());
	average.add(100);
	printf("%u\n",average.getAverage());
	average.add(5);
	printf("%u\n",average.getAverage());
	average.add(0);
	printf("%u\n",average.getAverage());


}
