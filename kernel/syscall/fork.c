#include "../processManager/processManager.h"
#include "../processManager/transfer.h"
#include "../processManager/processDescriptor.h"
#include "../mem/allocationTable.h"
#include "../../utils/string/string.h"
#include "../../utils/linkedList.h"

/* duplicate process and return child PID (in both) */
unsigned int fork(void)
{
	struct processDescriptor* current = getCurrentProcess();
	
	/* Copy original stack into a new one. */
	void* newStack = allocateMemory(current->stackSize, current->pid);
	memcpy(current->stack, newStack, current->stackSize);

	/* create a new process and fill parameters based on original */
	struct processDescriptor * process = createEmptyProcess()->element;
	process->stackSize = current->stackSize;

	/* Reset info stored so we can use them to prepare new process */
	saveProcessState(&current->processState);
	memcpy(&(current->processState), &(process->processState),
			sizeof(struct processState));
	
	int spOffset = current->stack - current->processState.sp;
	process->processState.sp = process->stack - spOffset;

	/* At this point, we just have to update the value in 
	   process->processState.pc such that when we start it we directly 
	   return 0. To do that, we have to change the value of 
	   process->processState.pc . */
	void** loc = &process->processState.pc;
	__asm__("str pc, [%0, #0]"
		:
		: "r" (loc)
	);

	return process->pid;
}
