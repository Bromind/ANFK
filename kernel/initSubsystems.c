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
	
void initSubsystems(void)
{
	LOG("Initialising kernel Memory");
	initKernelMemory();
	LOG("Initialising filesystem");
	initFS();
	LOG("Initialising processManager");
	initManager();
}

void startScheduler(void)
{
	LOG("Starting kernel");
	startKernel();
}
