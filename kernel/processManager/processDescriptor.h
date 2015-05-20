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
	unsigned int pid;
	unsigned int ppid;
	struct processState processState;
	struct memoryMap map;
};
