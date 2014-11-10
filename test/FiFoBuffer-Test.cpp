#include <stdio.h>
#include <inttypes.h>
#include "../FiFoBuffer.h"

const uint8_t size = 3;

FiFoBuffer<size,uint8_t> buffer(99);

int main( int argc, const char* argv[] ) {

	printf("FillCount: %u\n",buffer.getfillCount());
	printf("Empty: %u\n",buffer.isEmpty());
	printf("Full: %u\n\n", buffer.isFull());

	for (uint8_t index=0; index<size+2; index++) {
		buffer.add(index);
		printf("Added %u\n",index);
		printf("FillCount: %u\n",buffer.getfillCount());
		printf("Empty: %u\n",buffer.isEmpty());
		printf("Full: %u\n\n", buffer.isFull());

	}

	for (uint8_t index=0; index<size+2; index++) {
			uint8_t val = buffer.get();
			printf("Read: %u\n",val);
			printf("FillCount: %u\n",buffer.getfillCount());
			printf("Empty: %u\n",buffer.isEmpty());
			printf("Full: %u\n\n", buffer.isFull());

		}


}
