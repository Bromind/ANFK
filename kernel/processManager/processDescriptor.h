#ifndef STRUCT_PROCESSSTATE_H
#include "processState.h"
#define STRUCT_PROCESSSTATE_H
#endif

struct processDescriptor
{
	void* stack;
	unsigned int stackSize;
	unsigned int pid;
	unsigned int ppid;
	struct processState processState;
};
