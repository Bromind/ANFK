#ifndef PROCESSMANAGER_H
#include "../processManager/processManager.h"
#define PROCESSMANAGER_H
#endif

#ifndef TRANSFER_H
#include "../processManager/transfer.h"
#define TRANSFER_H
#endif

#ifndef PROCESSDESCRIPTOR_H
#include "../processManager/processDescriptor.h"
#define PROCESSDESCRIPTOR_H
#endif

#ifndef ALLOCATIONTABLE_H
#include "../mem/allocationTable.h"
#define ALLOCATIONTABLE_H
#endif

#ifndef STRING_H
#include "../../utils/string/string.h"
#define STRING_H
#endif

#ifndef LINKEDLIST_H
#include "../../utils/linkedList.h"
#define LINKEDLIST_H
#endif

/* duplicate process and return child PID (in both) */
unsigned int sys_fork(void)
{
	struct processDescriptor* current = getCurrentProcess();
	
	/* Copy original stack into a new one. */
	void* newStack = allocateMemory(current->stackSize, current->pid);
	memcpy(current->stack, newStack, current->stackSize);

	/* create a new process and fill parameters based on original */

	struct cell* cell = createEmptyProcess();

	struct processDescriptor * process = cell->element;
	process->stackSize = current->stackSize;
	process->stack = newStack;

	/* Reset info stored so we can use them to prepare new process */
	saveProcessState(&current->processState);
	
	int spOffset = current->stack - current->processState.sp;

	int fpOffset = current->stack - current->processState.r11;

	memcpy(&(current->processState), &(process->processState),
			sizeof(struct processState));

	process->processState.sp = process->stack - spOffset;
	process->processState.r11 = process->stack - fpOffset;


	savePC(&current->processState);
	memcpy(&(process->processState.pc), &(process->processState.pc), 
			sizeof(void *));
	if(process->pid != getPID())
	/* Put the new process in the running list. */
	{
		start(cell);
	}

	/* At this point, we just have to update the value in 
	   process->processState.pc such that when we start it we directly 
	   return 0. To do that, we have to change the value of 
	   process->processState.pc . */
	/*void** loc = &process->processState.pc;
	__asm__("str pc, [%0, #0]"
		:
		: "r" (loc)
	);*/

	return process->pid;
}
