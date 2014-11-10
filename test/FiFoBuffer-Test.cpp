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
		bool success = buffer.add(index);
		printf("Added %u\n",index);
		printf("Succesfull: %u\n",success);
		printf("FillCount: %u\n",buffer.getfillCount());
		printf("Empty: %u\n",buffer.isEmpty());
		printf("Full: %u\n\n", buffer.isFull());

	}

	for (uint8_t index=0; index<size+2; index++) {
			uint8_t val;
			bool success = buffer.get(val);
			if (success ) printf("Read: %u\n",val);
			else printf("Empty");
			printf("FillCount: %u\n",buffer.getfillCount());
			printf("Empty: %u\n",buffer.isEmpty());
			printf("Full: %u\n\n", buffer.isFull());

	}


}
