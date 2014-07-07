#include <stdlib.h>
#include "processManager.h"
#include "transfer.h"

void* running = NULL;

void* createStack(void* stack, void* pc, int stackSize)
{
	*((void**)stack) = pc;
	return stack;
}

void* createProcess(void (*f)(void), void* stack, int stackSize)
{
	void* pc = f;
	/* PC ; SP ; stackSize ; Register bank */
	int size = 2*REGISTER_SIZE + stackSize + NBR_REGISTER*REGISTER_SIZE;
	void* process = createStack(stack, pc, size);
	return process;
}


void switchToProcess(void* process)
{
	if (running == NULL)
	{
		startProcess(process); 
	} else {
		transfer(process, running);
	}
}
