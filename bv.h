//bv.h
//code written by Darrell Long found on assignment 1 PDF

# ifndef _BVector
# define _BVector
# include <stdint.h>

typedef struct bitV
{
	uint8_t *vector;
	uint32_t length;
} bitV;

bitV *newVec(uint32_t); 

void delVec(bitV *);

void oneVec(bitV *);

void setBit(bitV *, uint32_t);

void clrBit(bitV *, uint32_t);

uint32_t valBit(bitV *, uint32_t);
	
uint32_t lenVec(bitV *);
# endif
