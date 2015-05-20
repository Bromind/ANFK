
#ifndef ALLOCATION_TABLE_H
#include "../mem/allocationTable.h"
#define ALLOCATION_TABLE_H
#endif

#ifndef FREEMEMORY_H
#define FREEMEMORY_H
#include "../mem/freeMemory.h"
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

#ifndef STRUCT_MEMORYMAP_H
#include "../mem/memoryMap.h"
#define STRUCT_MEMORYMAP_H
#endif


struct linkedList * stoppedList;
struct linkedList * runningList;

unsigned int pidCounter = 0;

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

struct processDescriptor * getCurrentProcess(void)
{
	struct processDescriptor * current = (struct processDescriptor *)
		getIndex(runningList, 0)->element;
	return current;
}

unsigned int getPID(void)
{
	return ((struct processDescriptor*)getIndex(runningList, 0)->element)->pid;
}

unsigned int choosePPID(void)
{
	/* if first process */
	if(pidCounter == 0)
	{
		return 0;
	} else {
		struct processDescriptor* parent = (struct processDescriptor* )
			getIndex(runningList, 0)->element;
		return parent->pid;
	}
}

/* create new process and returns the cell containing the process */
struct cell * createProcess(void (*f)(void), void* baseAddress)
{
	void* area = get2M();
	/* Prepare initial processState*/
	struct processDescriptor * process =
		kalloc(sizeof(struct processDescriptor));
	
	process->processState.pc = f;
	process->processState.sp =  area + 2*SPACE - 4;
	/* When exiting, auto-delete process */
	process->processState.lr = &deleteProcess; 
	process->ppid = choosePPID();
	pidCounter++;
	process->pid = pidCounter;
	process->map.baseAddress = area;
	process->baseAddress = baseAddress;
	insertAtEnd(stoppedList, process);
	return getIndex(stoppedList, 0);
}

struct cell * createEmptyProcess(void)
{
	struct processDescriptor * process = 
		kalloc(sizeof(struct processDescriptor));
	process->ppid = choosePPID();
	pidCounter++;
	process->pid = pidCounter;
	insertAtEnd(stoppedList, process);
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
	free2M(process->map.baseAddress);
	if(process->baseAddress) /* If stored in kernel memory*/
	{
		kfree(process->baseAddress);
	}
	kfree(process);
	struct processDescriptor* next = 
		(struct processDescriptor *) getIndex(runningList, 0)->element;
	restartProcess(&(next->processState));
}

/* Start the process in the given cell (do not verify if the cell is indeed in 
 * the stoppedList), i.e.: add it to the running list. The process may not 
 * run immediately*/
void start(struct cell * processCell)
{
	void* process = processCell->element;
	removeCell(stoppedList, processCell);
	insertAtEnd(runningList, process);
}

/* Stop the process in the given cell (do not verify if the cell is indeed in 
 * the runningList), i.e.: remove it from the running. The process won't be 
 * running until it start again */
void stop(struct cell * processCell)
{
	void* process = processCell->element;
	removeCell(runningList, processCell);
	insertAtEnd(stoppedList, process);
	yield(); /* Yield in case the processus have the processor */
}

/* Suspend the current running process, and launch a running process. May be the
 * same if no other process are currently running*/
void yield(void)
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
	if (isEmpty(runningList))
	{
		startProcess(process); 
	} else {
		transfer(process, getIndex(runningList, 0)->element);
	}
}

/*
 * Instanciate Lists & idle process
 */
void initManager(void)
{
	stoppedList = newList();
	runningList = newList();

	struct cell * idleCell = createProcess(&idleProcess, 0);
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
