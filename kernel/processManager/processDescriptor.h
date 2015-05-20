#ifndef STRUCT_MEMORYMAP_H
#include "../mem/memoryMap.h"
#define STRUCT_MEMORYMAP_H
#endif

#ifndef STRUCT_PROCESSSTATE_H
#include "processState.h"
#define STRUCT_PROCESSSTATE_H
#endif

struct processDescriptor
{
	/* save the first address so we can free the file when exiting. 
	   Set to 0 if the program is compiled with the kernel. */
	void* baseAddress;
	unsigned int pid;
	unsigned int ppid;
	struct processState processState;
	struct memoryMap map;
};
