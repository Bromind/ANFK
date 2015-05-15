
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
void * allocateMemory(unsigned int size, unsigned int process);
void freeMemory(void* toFree, unsigned int process);
#ifdef DEBUG
void initFreeSpace(void);
#endif

/*---------------- Structure Definition --------------------*/

