#include <stdio.h>

//exclusive or
unsigned char checksum8_xor(void *data, int offset, int length)
{
	unsigned char checksum = 0;
	unsigned char *checksum_ptr = data;
	int i;
	
	for(i=offset; i < (offset + length);i++) {
		checksum ^= checksum_ptr[i];
	}
	//printf("checksum = %#02x\n", checksum);
	return checksum;
}

//summation
unsigned char checksum8_sum(void *data, int offset, int length)
{
	unsigned char checksum = 0;
	unsigned char *checksum_ptr = data;
	int i;
	
	for(i=offset; i < (offset + length);i++) {
		checksum += checksum_ptr[i];
	}
	//printf("checksum = %#02x\n", checksum);
	return checksum;
}

