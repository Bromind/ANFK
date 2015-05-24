#ifndef STRUCT_MEMORYMAP_H
#define STRUCT_MEMORYMAP_H
#include "memoryMap.h"
#endif

#ifndef LOGGER_H
#include "../logger.h"
#define LOGGER_H
#endif

#ifdef DEBUG

#ifndef MMAN
#include <sys/mman.h>
#define MMAN
#endif

#ifndef STDLIB
#include <stdlib.h>
#define STDLIB
#endif

#ifndef STDIO
#include <stdio.h>
#define STDIO
#endif

#ifndef ERRNO
#include <errno.h>
#define ERRNO
#endif

#ifndef UNISTD
#include <unistd.h>
#define UNISTD
#endif

#define MMAP_SIZE 335544320/*33554432*/ /* 2^21 * 8 bit/char * 20 char*/
void* freeSpace ;

#else

#include "base.h"
#endif
#define OFFSET_BY_CELL 16777216
#define OFFSET_0 0
#define OFFSET_1 2097152
#define OFFSET_2 4194304
#define OFFSET_3 6291456
#define OFFSET_4 8388608
#define OFFSET_5 10485760
#define OFFSET_6 12582912
#define OFFSET_7 14680064

/* if in debug, freeSpace points to malloc-ed space, o/w it is a symbol at 
   the beginning of the freeSpace, i.e.: the address of the first free space. */
#ifdef DEBUG
#define BASE_ADDRESS freeSpace
#else
#define BASE_ADDRESS &freeSpace
#endif

char blocks[2] = {0, 0};

void initFreeSpace(void)
{
#ifdef DEBUG
	void* addr = 0; /*malloc(MMAP_SIZE);*/
	freeSpace = addr;
	/*int fd = -1;
	freeSpace = mmap(addr,
			MMAP_SIZE,
			PROT_READ|PROT_WRITE|PROT_EXEC,
			MAP_SHARED|MAP_ANON,
			fd,
			0);
	if(freeSpace == MAP_FAILED)
	{
		printf("Failure : %i\n", errno);
		exit(-1);
	}*/
#else 
	LOG("Initialising freeSpace : ");
	freeSpace = BASE_ADDRESS; /* Set freeSpace to contain its own place */
	LOG_INT((int) freeSpace);
	LOG_CONT("\n");
#endif
}

/* if returns an address that is not multiple of 2 => error */
void* get2M(void)
{
	/*LOG("Allocating 2MiB\n");
	LOG("current block array : ");
	LOG_INT((int)(*blocks));
	LOG_CONT("\n");
	LOG("Freespace address : ");
	LOG_INT((int)BASE_ADDRESS);
	LOG_CONT("\n");*/
	LOG("Asking for 2MiB, returning address :");
	int i;
	for(i = 0 ; i < 2 ; i++)
	{
		char mask = 1 << 7;
		if(!(blocks[i] & mask)) 
		{
			LOG_INT((int) (OFFSET_BY_CELL*i + OFFSET_0 + BASE_ADDRESS));
			LOG_CONT("\n");
			blocks[i] |= mask;
			return OFFSET_BY_CELL*i + OFFSET_0 + BASE_ADDRESS;
		}
		mask = mask >>  1;
		if(!(blocks[i] & mask)) 
		{
			LOG_INT((int) (OFFSET_BY_CELL*i + OFFSET_1 + BASE_ADDRESS));
			LOG_CONT("\n");
			blocks[i] |= mask;
			return OFFSET_BY_CELL*i + OFFSET_1 + BASE_ADDRESS;
		}
		mask = mask >>  1;
		if(!(blocks[i] & mask)) 
		{
			LOG_INT((int) (OFFSET_BY_CELL*i + OFFSET_2 + BASE_ADDRESS));
			LOG_CONT("\n");
			blocks[i] |= mask;
			return OFFSET_BY_CELL*i + OFFSET_2 + BASE_ADDRESS;
		}
		mask = mask >>  1;
		if(!(blocks[i] & mask)) 
		{
			LOG_INT((int) (OFFSET_BY_CELL*i + OFFSET_3 + BASE_ADDRESS));
			LOG_CONT("\n");
			blocks[i] |= mask;
			return OFFSET_BY_CELL*i + OFFSET_3 + BASE_ADDRESS;
		}
		mask = mask >>  1;
		if(!(blocks[i] & mask)) 
		{
			LOG_INT((int) (OFFSET_BY_CELL*i + OFFSET_4 + BASE_ADDRESS));
			LOG_CONT("\n");
			blocks[i] |= mask;
			return OFFSET_BY_CELL*i + OFFSET_4 + BASE_ADDRESS;
		}
		mask = mask >>  1;
		if(!(blocks[i] & mask)) 
		{
			LOG_INT((int) (OFFSET_BY_CELL*i + OFFSET_5 + BASE_ADDRESS));
			LOG_CONT("\n");
			blocks[i] |= mask;
			return OFFSET_BY_CELL*i + OFFSET_5 + BASE_ADDRESS;
		}
		mask = mask >>  1;
		if(!(blocks[i] & mask)) 
		{
			LOG_INT((int) (OFFSET_BY_CELL*i + OFFSET_6 + BASE_ADDRESS));
			LOG_CONT("\n");
			blocks[i] |= mask;
			return OFFSET_BY_CELL*i + OFFSET_6 + BASE_ADDRESS;
		}
		mask = mask >>  1;
		if(!(blocks[i] & mask)) 
		{
			LOG_INT((int) (OFFSET_BY_CELL*i + OFFSET_7 + BASE_ADDRESS));
			LOG_CONT("\n");
			blocks[i] |= mask;
			return OFFSET_BY_CELL*i + OFFSET_7 + BASE_ADDRESS;
		}
		mask = mask >>  1;
	}
	return 0;

}

void free2M(void* ptr)
{
	int i = ((int)ptr)/OFFSET_BY_CELL;
	char mask = 1 << 7;
	switch(((int)ptr)%OFFSET_BY_CELL){
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
