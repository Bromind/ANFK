#ifndef TRANSFER_H
#include "../processManager/transfer.h"
#define TRANSFER_H
#endif

#ifndef FS_H
#include "../fs/fs.h"
#define FS_H
#endif 

#ifndef ALLOCATION_TABLE_H
#include "../mem/allocationTable.h"
#define ALLOCATION_TABLE_H
#endif

#ifndef PROCESS_MANAGER_H
#include "../processManager/processManager.h"
#define PROCESS_MANAGER_H
#endif 

#ifndef STRUCT_PROCESSDESCRIPTOR_H
#include "../processManager/processDescriptor.h"
#define STRUCT_PROCESSDESCRIPTOR_H
#endif 

/* DO NOT IMPLEMENT args SUPPORT NOW, THE file SHOULD BE IN THE CURRENT 
   DIRECTORY*/

unsigned int link(char* input, char* outpur, unsigned int inputLength);

/* fileName and args must be null-terminating */
int sys_exec(char* fileName, char* args)
{
	unsigned int nameLength = 0;
	while(*(fileName+nameLength)) nameLength++;

	struct fileDescriptor* fd = open(fileName, nameLength);
	unsigned int fileLength = fd->length;
	char* fileBuffer = kalloc(fileLength);

	read(fd, fileBuffer, fileLength);

	
	struct processDescriptor* current = getCurrentProcess();
	if(current->baseAddress)
	{
		kfree(current->baseAddress);
	}
	current->baseAddress = (void*) fileBuffer;
	current->processState.pc = (void*) fileBuffer;
	current->processState.sp = current->map.baseAddress + 2*SPACE - 4;
	current->processState.lr = &deleteProcess;
	{
		int i;
		for(i = 0 ; i < 2*NUMBER_OF_SECTION ; i++)
		{
			current->map.allocationTree[i].info = 0;
		}
	}
	startProcess(&current->processState);
	return 0;
}

unsigned int link(char* relocatableFile, char* output, unsigned int inputLength)
{
	output = relocatableFile;
	return inputLength;
}
