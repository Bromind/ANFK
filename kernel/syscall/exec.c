#include "../processManager/transfer.h"
#include "../fs/fs.h"
#include "../mem/allocationTable.h"
#include "../processManager/processManager.h"
#include "../processManager/processDescriptor.h"

/* DO NOT IMPLEMENT args SUPPORT NOW, THE file SHOULD BE IN THE CURRENT 
   DIRECTORY*/

unsigned int link(char* input, char* outpur, unsigned int inputLength);

/* fileName and args must be null-terminating */
int sys_exec(char* fileName, char* args)
{
	char tmp;
	unsigned int nameLength = 0;
	while(tmp = *(fileName+nameLength)) nameLength++;

	struct fileDescriptor* fd = open(fileName, nameLength);
	unsigned int fileLength = fd->length;
	char* fileBuffer = kalloc(fileLength);

	read(fd, fileBuffer, fileLength);

	char* located;
	unsigned int executableLength = link(fileBuffer, located, fileLength);
	
	struct processDescriptor* current = getCurrentProcess();
	if(current->baseAddress)
	{
		kfree(current->baseAddress);
	}
	current->baseAddress = (void*) located;
	current->processState.pc = (void*) located;
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

}

unsigned int link(char* relocatableFile, char* output, unsigned int inputLength)
{
	output = relocatableFile;
	return inputLength;
}
