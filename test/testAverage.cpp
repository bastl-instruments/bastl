#include <stdio.h>
#include <inttypes.h>


#include "../movingAverage.h"


MovingAverageLinear<uint16_t> average(4);



int main( int argc, const char* argv[] ) {


	printf("Pulse in\n");
	printf("%u\n",average.getAverage());
	average.add(100);
	printf("%u\n",average.getAverage());
	average.add(0);
	printf("%u\n",average.getAverage());
	average.add(0);
	printf("%u\n",average.getAverage());
	average.add(0);
	printf("%u\n",average.getAverage());
	average.add(0);
	printf("%u\n",average.getAverage());

	printf("\n");
	printf("Add new element\n");
	average.add(100);
	printf("%u\n",average.getAverage());
	printf("Clear and add same element\n");
	average.clear();
	average.add(100);
	printf("%u\n",average.getAverage());
	printf("Only clear and read\n");
	average.clear();
	printf("%u\n",average.getAverage());

	printf("\n");
	printf("Big Values\n");
	average.add(2277);
	printf("%u\n",average.getAverage());
	average.add(2521);
	printf("%u\n",average.getAverage());
	average.add(2317);
	printf("%u\n",average.getAverage());
	average.add(2399);
	printf("%u\n",average.getAverage());





}
