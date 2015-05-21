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
	
void initSubsystems(void)
{
	initKernelMemory();
	initFS();
	initManager();
}

void startScheduler(void)
{
	startKernel();
}
