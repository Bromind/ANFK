#ifndef INIT_KERNEL_MEMORY
#include "mem/initKernelMemory.h"
#define INIT_KERNEL_MEMORY
#endif

#ifndef INIT_PROCESS_MANAGER
#include "processManager/initProcessManager.h"
#define INIT_PROCESS_MANAGER
#endif

#ifndef INIT_FS
#include "fs/initFS.h"
#define INIT_FS
#endif 

#ifndef LOGGER_H
#include "logger.h"
#define LOGGER_H
#endif

#ifndef SYSTEM_TIMER_H
#include "driver/timer/systemTimer.h"
#define SYSTEM_TIMER_H
#endif
	
void initSubsystems(void)
{
	wait(10000000);
	LOG("Initialising kernel Memory");
	initKernelMemory();
	wait(10000000);
	LOG("Initialising filesystem");
	initFS();
	wait(10000000);
	LOG("Initialising processManager");
	initManager();
}

void startScheduler(void)
{
	wait(10000000);
	LOG("Starting kernel");
	startKernel();
}
