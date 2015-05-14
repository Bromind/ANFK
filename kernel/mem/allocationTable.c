#define DEBUG
/* Written after Tanenbaum explaination (Operating systems, section 4.2) of the 
   buddy System (Knuth 1973 ; Knowlton 1965)*/

#ifdef DEBUG 
/* simulate space by malloc-ing 1 MiB*/
#include<stdlib.h>
void* freeSpace;
#else 
#include "freeSpace.h"
#endif

#include "allocationTable.h"

/* We want a tree with NUMBER_OF_SECTION leaves, so 
   we need (2*NUMBER_OF_SECTION - 1) ~= (2*N_O_S) buddy */
/* We use the standard representation of a tree as an array (starting from 0) :
   - children of i are (i+1)*2 and (i+1)*2 - 1
   - parent of i is (i+1)/2 - 1 (integer div)
 */
struct buddy allocationTree[2*NUMBER_OF_SECTION];



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

void* allocateMemory(unsigned int size, int runningProcess)
{
	unsigned int pageSize = smallest2PowerAbove(size);
	void* address = 0;
	
	unsigned int index = indexOfFirstOfSize(pageSize);
	unsigned int maxIndex;
	if(pageSize = PAGE_SIZE) 
	{
		maxIndex = 2*NUMBER_OF_SECTION;
	} else {
		maxIndex = indexOfFirstOfSize(pageSize >> 1) - 1 ;
	}

	while (isNonEmpty(index))
	{
		index++;
		if(index >= maxIndex)
			goto no_place;
	}
	allocateBuddy(index, runningProcess);
	splitBuddy((index+1)/2 - 1);

	/* If the address has never been initialized, compute it*/
	if(address == 0){
		allocationTree[index].address =
			freeSpace + offsetFromIndex(index);
	}
	address = allocationTree[index].address;
	return address;

no_place:
	return 0;
}

/*We should free the biggest buddy at the given address which is not splitted */
void freeMemory(void* address, int runningProcess)
{
	unsigned int offset = (unsigned int) (address - freeSpace);
	/* if not a multiple of 64*/
	if(offset != ((offset>>(LOG_PAGE_SIZE - 1))<<(LOG_PAGE_SIZE - 1)))
		goto bad_offset;
	
	unsigned int index = smallestIndexFromOffset(offset);

	while(isSplitted(index))
	{
		index = (index+1)*2 - 1;
		if(index >= NUMBER_OF_SECTION)
			goto smallest_splitted;
	}
	if(allocationTree[index].process != runningProcess)
		goto bad_process;

	freeBuddy(index);
	mergeBuddy((index+1)/2 - 1);
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
}

void freeBuddy(unsigned int index)
{
	/* free sub-buddies */
	if(index < NUMBER_OF_SECTION -1)
	{
		freeBuddy((index+1)*2 - 1);
		freeBuddy((index+1)*2);
	}
	allocationTree[index].process = 0;
	allocationTree[index].info = 0;
}

void mergeBuddy(unsigned int index){
	/* If both children are empty, then it is empty and not splitted*/
	if(!isNonEmpty((index+1)*2 - 1)&& !isNonEmpty((index+1)*2))
	{
		allocationTree[index].info &= (EMPTY & NON_SPLITTED);
	}
	/* The buddy can't be full, since we merge after a free */
	allocationTree[index].info &= NON_FULL;

	/* Merge parent */
	if(index > 0)
	{
		mergeBuddy((index+1)/2 - 1);
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
void* allocateBuddy(unsigned int index, int process){
	/* allocate sub-buddies */
	/* First half of the array, i.e. not a leaf*/
	if(index < NUMBER_OF_SECTION - 1) 	{
		allocateBuddy((index+1)*2 - 1, process);
		allocateBuddy((index+1)*2, process);
	}
	allocationTree[index].process = process;
	allocationTree[index].info |= NON_EMPTY | FULL;
	allocationTree[index].info &= NON_SPLITTED;

	return allocationTree[index].address;
}

/* Split the buddy (and all super-buddies) */
void splitBuddy(unsigned int index){
	allocationTree[index].info |= SPLITTED | NON_EMPTY;
	
	/* if both children are full, it is full */
	if(isFull((index+1)*2 -1) && isFull((index+1)*2)) 
	{
		allocationTree[index].info |= FULL;
	}
	
	/* Split super-buddies */
	if(index > 0) /*i.e.: not the top level*/
	{
		splitBuddy((index+1)/2 - 1);
	}
}

/* Bit of magic here, you should trust in magic...
 */
unsigned int indexOfFirstOfSize(unsigned int n)
{
	return (NUMBER_OF_SECTION / (n >> (LOG_PAGE_SIZE - 1 ))) - 1;
}

char isFull(unsigned int index){
	return allocationTree[index].info & FULL;
}

char isNonEmpty(unsigned int index){
	return allocationTree[index].info & NON_EMPTY;
}

char isSplitted(unsigned int index){
	return allocationTree[index].info & SPLITTED; 
}

#ifdef DEBUG
#include<stdio.h>

void printBuddy(unsigned int index, unsigned int level)
{
	{
		unsigned int l = level;
		for(; l > 1 ; l--)
		{
			printf("|   ");
		}
	}
	if(index == 0){
		printf("%p, %x, process n°%i\n", 
				allocationTree[index].address, 
				allocationTree[index].info,
				allocationTree[index].process);
	} else {
		printf("+-- %p, %x, process n°%i\n", 
				allocationTree[index].address, 
				allocationTree[index].info,
				allocationTree[index].process);
	}
	if(index < NUMBER_OF_SECTION - 1)
	{
		printBuddy((index+1)*2 - 1, level + 1);
		printBuddy((index+1)*2, level + 1);
	}
}
void main(void)
{
	freeSpace = malloc(1048576);
	void* first = allocateMemory(64, 1);
	void* second = allocateMemory(128, 2);
	void* third = allocateMemory(262144, 3);
	printf("Base pointer : %p\n", freeSpace);
	printf("------- Allocate 256bytes --------\n");
//	printBuddy(0, 0);
	printf("------- Free third ---------------\n");
	freeMemory(second, 2);
	printBuddy(0, 0);
	free(freeSpace);
}

#endif 
