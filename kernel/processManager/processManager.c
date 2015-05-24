
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

#ifndef TIMER_H
#include "../driver/timer/systemTimer.h"
#define TIMER_H
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


struct linkedList stoppedList;
struct linkedList runningList;

unsigned int pidCounter = 0;

void idleProcess(void)
{
	while(1)
	{
		LOG("idle\n");
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
		getIndex(&runningList, 0)->element;
	return current;
}

unsigned int getPID(void)
{
	return ((struct processDescriptor*)getIndex(&runningList, 0)->element)->pid;
}

unsigned int choosePPID(void)
{
	/* if first process */
	if(pidCounter == 0)
	{
		return 0;
	} else {
		struct processDescriptor* parent = (struct processDescriptor* )
			getIndex(&runningList, 0)->element;
		return parent->pid;
	}
}

/* create new process and returns the cell containing the process */
struct cell * createProcess(void (*f)(void), void* baseAddress)
{
	void* area = get2M();
	LOG("Creating process at base address : ");
	LOG_INT((int)area);
	LOG_CONT("\n");
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
	LOG("Process created at address ");
	LOG_INT((int) process);
	LOG_CONT(" : \n");
	printProcess(process);
	insertAtEnd(&stoppedList, process);
	return getIndex(&stoppedList, 0)->previous;
}

struct cell * createEmptyProcess(void)
{
	struct processDescriptor * process = 
		kalloc(sizeof(struct processDescriptor));
	process->ppid = choosePPID();
	pidCounter++;
	process->pid = pidCounter;
	insertAtEnd(&stoppedList, process);
	return getIndex(&stoppedList, 0);
}

/*
 * Delete current process, i.e. remove from runningList, free the Cell, 
 * and switch process. Free the stack too.
 */
void deleteProcess()
{
	struct processDescriptor* process = (struct processDescriptor*)
		 removeCell(&runningList, getIndex(&runningList, 0));
	LOG("Deleting process : ");
	LOG_INT((int) process->pid);
	LOG_CONT("\n");
	free2M(process->map.baseAddress);
	if(process->baseAddress) /* If stored in kernel memory*/
	{
		kfree(process->baseAddress);
	}
	kfree(process);
	struct processDescriptor* next = 
		(struct processDescriptor *) getIndex(&runningList, 0)->element;
	restartProcess(&(next->processState));
}

/* Start the process in the given cell (do not verify if the cell is indeed in 
 * the stoppedList), i.e.: add it to the running list. The process may not 
 * run immediately*/
void start(struct cell * processCell)
{
	void* process = processCell->element;
	removeCell(&stoppedList, processCell);
	insertAtEnd(&runningList, process);
}

/* Stop the process in the given cell (do not verify if the cell is indeed in 
 * the runningList), i.e.: remove it from the running. The process won't be 
 * running until it start again */
void stop(struct cell * processCell)
{
	void* process = processCell->element;
	removeCell(&runningList, processCell);
	insertAtEnd(&stoppedList, process);
	yield(); /* Yield in case the processus have the processor */
}

/* Suspend the current running process, and launch a running process. May be the
 * same if no other process are currently running*/
void yield(void)
{
	LOG("Transfering processes (current, next) : ");
	struct processDescriptor* current = 
		(struct processDescriptor*) getIndex(&runningList, 0)->element;
	rotateForward(&runningList);
	struct processDescriptor* next = 
		(struct processDescriptor*) getIndex(&runningList, 0)->element;
	LOG_INT(current->pid);
	LOG_CONT("  ");
	LOG_INT(next->pid);
	LOG_CONT("\n");
	wait(1000000);
	transfer(&(next->processState), &(current->processState));
}

/*
 * Start given process and give it cpu
 */
void switchToProcess(void* process)
{
	if (isEmpty(&runningList))
	{
		startProcess(process); 
	} else {
		transfer(process, getIndex(&runningList, 0)->element);
	}
}

/*
 * Instanciate Lists & idle process
 */
void initManager(void)
{
	stoppedList.head = 0;
	stoppedList.size = 0;
	runningList.head = 0;
	runningList.size = 0;
	LOG("stoppedList at ");
	LOG_INT((int) &stoppedList);
	LOG_CONT("\n");
	LOG("runningList at ");
	LOG_INT((int) &runningList);
	LOG_CONT("\n");

	LOG("creating idle process\n");
	struct cell * idleCell = createProcess(&idleProcess, 0);
	start(idleCell);
}

/*
 * Launch the kernel, i.e. launch first process in the running list
 */
void startKernel(void)
{
	LOG("Gonna start the kernel, remind all processes :\n");
	LOG("runningList (size, head) : ");
	LOG_INT(runningList.size);
	LOG_INT((int)runningList.head);
	LOG_CONT("\n");
	
	int i = 0;
	int max = runningList.size;
	LOG("Number of processes : ");
	LOG_INT(max);
	LOG_CONT("\n");
	for( ; i < max ; i++)
	{
		struct processDescriptor* p = 
			(struct processDescriptor*) getIndex(&runningList, i)->element;
		printProcess(p);
	}
	struct processDescriptor* process = 
		(struct processDescriptor*) getIndex(&runningList, 0)->element;
	LOG("Starting process : ");
	LOG_INT(process->pid);
/*	LOG_INT(process->ppid); */
	printProcess(process);
	LOG_CONT("\n");
	startProcess(&(process->processState));
}

void printProcess(struct processDescriptor* process)
{
	LOG("Print process (map.baseAddress, pid, ppid, pc, sp, lr) :\n ");
	LOG_INT((int) process->map.baseAddress);
	LOG_CONT(" ");
	LOG_INT(process->pid);
	LOG_CONT(" ");
	LOG_INT(process->ppid);
	LOG_CONT(" ");
	LOG_INT((int)process->processState.pc);
	LOG_CONT(" ");
	LOG_INT((int)process->processState.sp);
	LOG_CONT(" ");
	LOG_INT((int)process->processState.lr);
	LOG_CONT("\n");
	wait(3000000);
}
