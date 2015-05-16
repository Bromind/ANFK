
#ifndef ALLOCATION_TABLE_H
#include "../mem/allocationTable.h"
#define ALLOCATION_TABLE_H
#endif

#ifndef LOGGER_H
#include "../logger.h"
#define LOGGER_H
#endif

#ifdef DEBUG
#ifndef STDIO_H
#include <stdio.h>
#define STDIO_H
#endif
#endif

#ifndef PROCESS_MANAGER_H
#include "processManager.h"
#define PROCESS_MANAGER_H
#endif

#ifndef TRANSFER_H
#include "transfer.h"
#define TRANSFER_H
#endif

#ifndef LINKEDLIST_H
#include "../../utils/linkedList.h"
#define LINKEDLIST_H
#endif

#define IDLE_STACK_SIZE 4096
#define PROCESS_MANAGER_ID 1
#define NBR_REGISTER 15
#define REGISTER_SIZE 4


#ifndef STRUCT_PROCESSDESCRIPTOR_H
#include "processDescriptor.h"
#define STRUCT_PROCESSDESCRIPTOR_H
#endif


void* running = NULL;

struct linkedList * stoppedList;
struct linkedList * runningList;

void* localStack;
void* sp_tmp;

void idleProcess(void)
{
	while(1)
	{
		#ifdef DEBUG
		LOG("idle");
		#endif
		yield();
	}
}

void* createStack(void* stack, void* pc, int stackSize)
{
	*((void**)stack) = pc;
	return stack;
}

/* create new process and returns the cell containing the process */
struct cell * createProcess(void (*f)(void), void* stackAddress, int stackSize)
{
	/* Prepare initial processState*/
	struct processDescriptor * process =
		allocateMemory(sizeof(struct processDescriptor), 
				PROCESS_MANAGER_ID);
	process->processState.pc = f;
	process->processState.sp = stackAddress + stackSize - 1;
	/* When exiting, auto-delete process */
	process->processState.lr_tmp = &deleteProcess; 
	/* Save stack allocation address to free it when removing the process*/
	process->stack = stackAddress;
	insert(stoppedList, process);
	return getIndex(stoppedList, 0);
}

/*
 * Delete current process, i.e. remove from runningList, free the Cell, 
 * and switch process. Free the stack too.
 */
void deleteProcess()
{
	struct processDescriptor* process = (struct processDescriptor*)
		 removeCell(runningList, getIndex(runningList, 0));
	freeMemory(process->stack, PROCESS_MANAGER_ID);
	freeMemory(process, PROCESS_MANAGER_ID);
	void* next = getIndex(runningList, 0)->element;
	restartProcess(next);
}

/* Start the process in the given cell (do not verify if the cell is indeed in 
 * the stoppedList), i.e.: add it to the running list. The process may not 
 * run immediately*/
void start(struct cell * processCell)
{
	void* process = processCell->element;
	removeCell(stoppedList, processCell);
	insert(runningList, process);
}

/* Stop the process in the given cell (do not verify if the cell is indeed in 
 * the runningList), i.e.: remove it from the running. The process won't be 
 * running until it start again */
void stop(struct cell * processCell)
{
	void* process = processCell->element;
	removeCell(running, processCell);
	insert(stoppedList, process);
	yield(); /* Yield in case the processus have the processor */
}

/* Suspend the current running process, and launch a running process. May be the
 * same if no other process are currently running*/
void yield()
{
	struct processDescriptor* current = 
		(struct processDescriptor*) getIndex(runningList, 0)->element;
	rotateForward(runningList);
	struct processDescriptor* next = 
		(struct processDescriptor*) getIndex(runningList, 0)->element;
	transfer(&(next->processState), &(current->processState));
}

/*
 * Start given process and give it cpu
 */
void switchToProcess(void* process)
{
	if (running == NULL)
	{
		startProcess(process); 
	} else {
		transfer(process, running);
	}
}

/*
 * Instanciate Lists & idle process
 */
void initManager(void)
{
	stoppedList = newList();
	runningList = newList();

	void* idleStack = allocateMemory(IDLE_STACK_SIZE, PROCESS_MANAGER_ID);
	struct cell * idleCell = createProcess(&idleProcess, idleStack,
			IDLE_STACK_SIZE);
	start(idleCell);
}

/*
 * Launch the kernel, i.e. launch first process in the running list
 */
void startKernel(void)
{
	struct processDescriptor* process = 
		(struct processDescriptor*) getIndex(runningList, 0)->element;
	startProcess(&(process->processState));
}
