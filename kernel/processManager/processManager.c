#include <stdlib.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#include "processManager.h"
#include "transfer.h"

#define IDLE_STACK_SIZE 50

void* running = NULL;

struct linkedList * stoppedList;
struct linkedList * runningList;

void idleProcess(void)
{
	while(1)
	{
		#ifdef DEBUG
		printf("idle");
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
struct cell * createProcess(void (*f)(void), void* stack, int stackSize)
{
	void* pc = f;
	/* PC ; SP ; stackSize ; Register bank */
	int size = 2*REGISTER_SIZE + stackSize + NBR_REGISTER*REGISTER_SIZE;
	void* process = createStack(stack, pc, size);
	insert(stoppedList, process);
	return getIndex(stoppedList, 0);
}

/*
 * Delete current process, i.e. remove from runningList, free the Cell, 
 * and switch process
 */
void deleteProcess()
{
	void* process = removeCell(runningList, getIndex(runningList, 0));
	free(process);
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
	void* current = getIndex(runningList, 0)->element;
	rotateForward(runningList);
	void* next = getIndex(runningList, 0)->element;
	transfer(next, current);
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

	void* idleStack = malloc(IDLE_STACK_SIZE);
	struct cell * idleCell = createProcess(&idleProcess, idleStack,
			IDLE_STACK_SIZE);
	start(idleCell);
}

/*
 * Launch the kernel, i.e. launch first process in the running list
 */
void startKernel(void)
{
	void* process = getIndex(runningList, 0)->element;
	startProcess(process);
}
