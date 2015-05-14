
/* Free space = 1MiB = 1048576 bytes */
#define SPACE 1048576

/* Smallest section is 64 bytes */
#define PAGE_SIZE 64
#define LOG_PAGE_SIZE 7

#define NUMBER_OF_SECTION SPACE/PAGE_SIZE

#define FULL 4 
#define NON_FULL -5
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
unsigned int offsetFromIndex(unsigned int index);
unsigned int smallestIndexFromOffset(unsigned int offset);
unsigned int removeRightZeros(unsigned int value);
void freeBuddy(unsigned int index);
void mergeBuddy(unsigned int index);
char isSplitted(unsigned int index);

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
