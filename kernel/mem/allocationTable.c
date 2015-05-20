/* Written after Tanenbaum explaination (Operating systems, section 4.2) of the 
   buddy System (Knuth 1973 ; Knowlton 1965)*/

#ifdef DEBUG 
/* simulate space by malloc-ing 1 MiB*/

#ifndef STDLIB
#include<stdlib.h>
#define STDLIB
#endif

#endif

#include "freeMemory.h"

#include "allocationTable.h"

#define FULL 4 
#define NON_FULL -5
#define EMPTY -3
#define NON_EMPTY 2
#define SPLITTED 1
#define NON_SPLITTED -2
#define MAX_OFFSET 1048512

/*------------------ Internal Structures -----------------------------*/


/*------------------ Internal Functions ------------------------------*/
void * allocateMemory(unsigned int size, struct memoryMap* map);
void freeMemory(void* toFree, struct memoryMap* map);
void allocateBuddy(unsigned int index, struct memoryMap* map);
unsigned int indexOfFirstOfSize(unsigned int n);
void splitBuddy(unsigned int index, struct memoryMap* map);
char isFull(unsigned int index, struct memoryMap* map);
char isNonEmpty(unsigned int index, struct memoryMap* map);
unsigned int offsetFromIndex(unsigned int index);
unsigned int smallestIndexFromOffset(unsigned int offset);
unsigned int removeRightZeros(unsigned int value);
void freeBuddy(unsigned int index, struct memoryMap* map);
void mergeBuddy(unsigned int index, struct memoryMap* map);
char isSplitted(unsigned int index, struct memoryMap* map);
unsigned int smallest2PowerAbove(unsigned int x);
unsigned int biggest2PowerUnder(unsigned int x);
#ifdef DEBUG
void printBuddy(unsigned int index, unsigned int level, struct memoryMap* map);
#endif

/*---------------- Global variables ---------------------------------*/
struct memoryMap kernelAllocationTree;

void* kalloc(unsigned int size)
{
	return allocateMemory(size, &kernelAllocationTree);
}

void kfree(void* ptr)
{
	freeMemory(ptr, &kernelAllocationTree);
}

/*Assuming 32 bits arch, from Hacker's delight, Warren */
unsigned int smallest2PowerAbove(unsigned int x){
	x = x-1;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	return x+1;
}

unsigned int biggest2PowerUnder(unsigned int x)
{
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	return x - (x >> 1);
}

void* allocateMemory(unsigned int size, struct memoryMap* map)
{
	if(size == 0)
	{
		return 0;
	}
	unsigned int pageSize = smallest2PowerAbove(size);
	pageSize = (pageSize >= 64 ? pageSize : 64);
	
	unsigned int index = indexOfFirstOfSize(pageSize);
	unsigned int maxIndex;
	if(pageSize == PAGE_SIZE) 
	{
		maxIndex = 2*NUMBER_OF_SECTION;
	} else {
		maxIndex = indexOfFirstOfSize(pageSize >> 1) - 1 ;
	}

	while (isNonEmpty(index, map))
	{
		index++;
		if(index >= maxIndex)
			goto no_place;
	}
	allocateBuddy(index, map);
	splitBuddy((index+1)/2 - 1, map);

	return map->baseAddress + offsetFromIndex(index);

no_place:
	return 0;
}

/*We should free the biggest buddy at the given address which is not splitted */
void freeMemory(void* address, struct memoryMap* map)
{
	if(address == 0) return;
	unsigned int offset = (unsigned int) (address - map->baseAddress);
	/* if not a multiple of 64*/
	if(offset != ((offset>>(LOG_PAGE_SIZE - 1))<<(LOG_PAGE_SIZE - 1)))
		goto bad_offset;
	if(offset > MAX_OFFSET) goto bad_process;
	
	unsigned int index = smallestIndexFromOffset(offset);

	while(isSplitted(index, map))
	{
		index = (index+1)*2 - 1;
		if(index >= NUMBER_OF_SECTION)
			goto smallest_splitted;
	}

	freeBuddy(index, map);
	mergeBuddy((index+1)/2 - 1, map);
	return;

smallest_splitted:
	return; /* TODO send error */
bad_process:
	return; /* TODO send error */
bad_offset:
	return; /* TODO send error */
}

unsigned int smallestIndexFromOffset(unsigned int offset)
{
	unsigned int virtOffset = offset >> (LOG_PAGE_SIZE - 1);
	if(virtOffset == 0) return 0;

	unsigned int rightMostOneBit = virtOffset & (-virtOffset);
	unsigned int shifted = removeRightZeros(virtOffset);

	unsigned int index = (2*NUMBER_OF_SECTION)/(rightMostOneBit << 1) -1 
		+ shifted;
	 return index;
}

unsigned int removeRightZeros(unsigned int value)
{
	if(value == 0) return 0;
	unsigned int n = 1;
	while(!(value & n)) value >>= 1;
	return value;
}

void freeBuddy(unsigned int index, struct memoryMap* map)
{
	/* free sub-buddies */
	if(index < NUMBER_OF_SECTION -1)
	{
		freeBuddy((index+1)*2 - 1, map);
		freeBuddy((index+1)*2, map);
	}
	map->allocationTree[index].info = 0;
}

void mergeBuddy(unsigned int index, struct memoryMap* map){
	/* If both children are empty, then it is empty and not splitted*/
	if(!isNonEmpty((index+1)*2 - 1, map)&& !isNonEmpty((index+1)*2, map))
	{
		map->allocationTree[index].info 
			&= (EMPTY & NON_SPLITTED);
	}
	/* The buddy can't be full, since we merge after a free */
	map->allocationTree[index].info &= NON_FULL;

	/* Merge parent */
	if(index > 0)
	{
		mergeBuddy((index+1)/2 - 1, map);
	}
}

unsigned int offsetFromIndex(unsigned int index)
{
	unsigned int nth = index + 1;
	unsigned int sectionSize = SPACE/biggest2PowerUnder(nth);
	/* Not the real address, doesn't take min page size into account*/
	unsigned int virtOffset = 
		(nth - biggest2PowerUnder(nth)) * sectionSize / PAGE_SIZE;
	/* Shift by the LOG_PAGE_SIZE to have the true address*/
	unsigned int offset = (virtOffset << (LOG_PAGE_SIZE-1));
	return offset;
}

/* Reserve the buddy (and all sub-buddies) */
void allocateBuddy(unsigned int index, struct memoryMap* map){
	/* allocate sub-buddies */
	/* First half of the array, i.e. not a leaf*/
	if(index < NUMBER_OF_SECTION - 1) 	{
		allocateBuddy((index+1)*2 - 1, map);
		allocateBuddy((index+1)*2, map);
	}
	map->allocationTree[index].info |= NON_EMPTY | FULL;
	map->allocationTree[index].info &= NON_SPLITTED;

	return;
}

/* Split the buddy (and all super-buddies) */
void splitBuddy(unsigned int index, struct memoryMap* map){
	map->allocationTree[index].info |= SPLITTED | NON_EMPTY;
	
	/* if both children are full, it is full */
	if(isFull((index+1)*2 - 1, map) && isFull((index+1)*2, map)) 
	{
		map->allocationTree[index].info |= FULL;
	}
	
	/* Split super-buddies */
	if(index > 0) /*i.e.: not the top level*/
	{
		splitBuddy((index+1)/2 - 1, map);
	}
}

/* Bit of magic here, you should trust in magic...
 */
unsigned int indexOfFirstOfSize(unsigned int n)
{
	return (NUMBER_OF_SECTION / (n >> (LOG_PAGE_SIZE - 1 ))) - 1;
}

char isFull(unsigned int index, struct memoryMap* map){
	return map->allocationTree[index].info & FULL;
}

char isNonEmpty(unsigned int index, struct memoryMap* map){
	return map->allocationTree[index].info & NON_EMPTY;
}

char isSplitted(unsigned int index, struct memoryMap* map){
	return map->allocationTree[index].info & SPLITTED; 
}

void initKernelMemory(void)
{
	kernelAllocationTree.baseAddress = get2M();
}

#ifdef DEBUG

#ifndef STDIO
#include<stdio.h>
#define STDIO
#endif

void printBuddy(unsigned int index, unsigned int level, struct memoryMap* map)
{
	{
		unsigned int l = level;
		for(; l > 1 ; l--)
		{
			printf("|   ");
		}
	}
	if(index == 0){
		printf("%x", 
			map->allocationTree[index].info);
	} else {
		printf("+-- %x", 
			map->allocationTree[index].info);
	}
	if(index < NUMBER_OF_SECTION - 1)
	{
		printBuddy((index+1)*2 - 1, level + 1, map);
		printBuddy((index+1)*2, level + 1, map);
	}
}

#ifdef TEST_ALLOCATION
void main(void)
{
	initFreeSpace();
	void* first = kalloc(64);
	void* second = kalloc(128);
	void* third = kalloc(262144);
	printf("Base pointer : %p\n", freeSpace);
	printf("------- Allocate 256bytes --------\n");
	printBuddy(0, 0, &kernelAllocationTree);
	printf("------- Free third ---------------\n");
	kfree(second);
	kfree(third);
	printBuddy(0, 0, &kernelAllocationTree);
	freeFreeSpace();
}
#endif

#endif 
