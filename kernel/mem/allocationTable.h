#ifndef STRUCT_MEMORYMAP_H
#include "memoryMap.h"
#define STRUCT_MEMORYMAP_H
#endif

/*---------------- Function Definition ---------------------*/
void* kalloc(unsigned int size);
void kfree(void* ptr);

void initKernelMemory(void);

