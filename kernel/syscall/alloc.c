#ifndef ALLOCATION_TABLE_H
#include "../mem/allocationTable.h"
#define ALLOCATION_TABLE_H
#endif

#ifndef STRUCT_PROCESSDESCRIPTOR_H
#include "../processManager/processDescriptor.h"
#define STRUCT_PROCESSDESCRIPTOR_H
#endif

#ifndef PROCESSMANAGER_H
#include "../processManager/processManager.h"
#define PROCESSMANAGER_H
#endif

#ifndef STRUCT_MEMORYMAP_H
#include "../mem/memoryMap.h"
#define STRUCT_MEMORYMAP_H
#endif

void* sys_alloc(unsigned int size)
{
	struct processDescriptor* desc = getCurrentProcess();
	struct memoryMap* map = &desc->map;

	return allocateMemory(size, map);
}

void sys_free(void* ptr)
{
	struct processDescriptor* desc = getCurrentProcess();
	struct memoryMap* map = &desc->map;
	freeMemory(ptr, map);
}
