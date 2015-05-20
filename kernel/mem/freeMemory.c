#ifndef STRUCT_MEMORYMAP_H
#define STRUCT_MEMORYMAP_H
#include "memoryMap.h"
#endif

#ifdef DEBUG
#ifndef STDLIB
#include <stdlib.h>
#define STDLIB
#endif

void* get2M(void)
{
	return malloc(2*SPACE);
}

void free2M(void* ptr)
{
	/*free(ptr);*/ /* Just let the memory leak, it is just a simulation 
			and it will segfault since we won't have de same stack*/
}
#else

#include "base.h"

#define OFFSET_BY_CELL 16777216
#define OFFSET_0 0
#define OFFSET_1 2097152
#define OFFSET_2 4194304
#define OFFSET_3 6291456
#define OFFSET_4 8388608
#define OFFSET_5 10485760
#define OFFSET_6 12582912
#define OFFSET_7 14680064
char blocks[20];

/* if returns an address that is not multiple of 2 => error */
void* get2M(void)
{
	int i;
	for(i = 0 ; i < 20 ; i++)
	{
		char mask = 1 << 7;
		if(!(blocks[i] & mask)) 
			return OFFSET_BY_CELL*i + OFFSET_0 + freeSpace;
		mask = mask >>  1;
		if(!(blocks[i] & mask)) 
			return OFFSET_BY_CELL*i + OFFSET_1 + freeSpace;
		mask = mask >>  1;
		if(!(blocks[i] & mask)) 
			return OFFSET_BY_CELL*i + OFFSET_2 + freeSpace;
		mask = mask >>  1;
		if(!(blocks[i] & mask)) 
			return OFFSET_BY_CELL*i + OFFSET_3 + freeSpace;
		mask = mask >>  1;
		if(!(blocks[i] & mask)) 
			return OFFSET_BY_CELL*i + OFFSET_4 + freeSpace;
		mask = mask >>  1;
		if(!(blocks[i] & mask)) 
			return OFFSET_BY_CELL*i + OFFSET_5 + freeSpace;
		mask = mask >>  1;
		if(!(blocks[i] & mask)) 
			return OFFSET_BY_CELL*i + OFFSET_6 + freeSpace;
		mask = mask >>  1;
		if(!(blocks[i] & mask)) 
			return OFFSET_BY_CELL*i + OFFSET_7 + freeSpace;
		mask = mask >>  1;
	}
	return 1;

}

void free2M(void* ptr)
{
	int i = ptr/OFFSET_BY_CELL;
	char mask = 1 << 7;
	switch(ptr%OFFSET_BY_CELL){
		case OFFSET_0:
			mask = -1 & (~mask);
			break;
		case OFFSET_1:
			mask = -1 & ~(mask >> 1);
			break;
		case OFFSET_2:
			mask = -1 & ~(mask >> 2);
			break;
		case OFFSET_3:
			mask = -1 & ~(mask >> 3);
			break;
		case OFFSET_4:
			mask = -1 & ~(mask >> 4);
			break;
		case OFFSET_5:
			mask = -1 & ~(mask >> 5);
			break;
		case OFFSET_6:
			mask = -1 & ~(mask >> 6);
			break;
		case OFFSET_7:
			mask = -1 & ~(mask >> 7);
			break;
	}
	blocks[i] = blocks[i] & mask;

}
#endif
