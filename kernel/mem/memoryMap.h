
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
#define MAX_OFFSET 1048512

/*---------------- Structure Definition --------------------*/

struct buddy {
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
struct memoryMap {
	void* baseAddress;
	struct buddy allocationTree[2*NUMBER_OF_SECTION];
};
