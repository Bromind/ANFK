#define DEBUG
/* Written after Tanenbaum explaination (Operating systems, sec 4.2) of the 
   buddy System (Knuth 1973 ; Knowlton 1965)*/

#ifndef DEBUG 
#include<freeSpace.h>
#endif

/* Free space = 1MiB = 1048576 bytes */
#define SPACE 1048576

/* Smallest section is 64 bytes */
#define PAGE_SIZE 64
#define LOG_PAGE_SIZE 7

#define NUMBER_OF_SECTION SPACE/PAGE_SIZE

#define FULL 4 
#define NO_FULL -5
#define EMPTY -3
#define NON_EMPTY 2
#define SPLITTED 1
#define NON_SPLITTED -2
/*---------------- Function Definition ---------------------*/
void* allocateBuddy(unsigned int index, int process);
unsigned int indexOfFirstOfSize(unsigned int n);
void splitBuddy(unsigned int index);
char isFull(unsigned int index);
char isNonEmpty(unsigned int index);

/*---------------- Structure Definition --------------------*/
struct buddy {
	void* address;
	int process;
	
	/* 
	   full (yes = 1/no=0) << 2 | 
	   empty (yes = 0 /no = 1) << 1 |
	   splitted (yes = 1/no = 0)
	 */
	char info;
};

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

void* allocateMemory(unsigned int size, int runningProcess)
{
	unsigned int pageSize = smallest2PowerAbove(size);
	void* address = 0;
	
	unsigned int index = indexOfFirstOfSize(pageSize);
	unsigned int maxIndex = indexOfFirstOfSize(pageSize >> 1) - 1 ;

	while (isNonEmpty(index))
	{
		index++;
		if(index >= maxIndex)
			goto no_place;
	}
	allocateBuddy(index, runningProcess);
	splitBuddy((index+1)/2 - 1);
	address = allocationTree[index].address;
#ifdef DEBUG
	return address;
#else
	return address + freeSpace;
#endif

no_place:
	return 0;
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
	allocateMemory(256, 1);
	allocateMemory(512, 2);
	allocateMemory(256, 3);
	printf("------- Allocate 256bytes --------\n");
	printBuddy(0, 0);
}

#endif 
