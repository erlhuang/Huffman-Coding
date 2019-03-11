//bv.c
//oliver mulcahy (opm@ucsc.edu)


#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct bitV
{
	uint8_t *vector;
	uint32_t length;
} bitV;

bitV *newVec(uint32_t len)
{
	//malloc memory
	bitV* vecName = malloc(sizeof(bitV));
	//assign new vec length
	vecName->length = len;
	//allocate space based on length for the vector
	vecName->vector = calloc(len/8+1, sizeof(uint8_t));

	return vecName;
}

void delVec(bitV* bv)
{
	//free the vector of the bv
	free(bv->vector); 
	//free the bv
	free(bv);
}

void oneVec(bitV* bv)
{
	//from man page for memset:
	//memset(void *b, int c, size_t len)
	//writes len bytes of char c to string b
	memset(bv->vector, 255, (bv->length)/8 + 1);
}

void setBit(bitV* bv, uint32_t bit)
{
	//code by Darrell Long on Piazza
	bv -> vector[bit >> 3] |= (01 << (bit & 07));
}

void clrBit(bitV* bv, uint32_t bit)
{
	//code by Darrell Long on Piazza
	bv -> vector[bit >> 3] &= ~(01 << (bit & 07));
}

uint32_t valBit(bitV* bv, uint32_t bit)
{
	uint32_t val;
	//code by Darrell Long on Piazza
	val = (bv -> vector[bit >> 3] & (01 << (bit & 07))) >> (bit & 07);
	return val;
}

uint32_t lenVec(bitV* bv)
{
	return bv->length;
}
