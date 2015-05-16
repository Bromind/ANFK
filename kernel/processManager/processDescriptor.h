#ifndef STRUCT_PROCESSSTATE_H
#include "processState.h"
#define STRUCT_PROCESSSTATE_H
#endif

struct processDescriptor
{
	void* stack;
	struct processState processState;
};
