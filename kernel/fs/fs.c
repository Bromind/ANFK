#include "../../utils/linkedList.h"
#include "../../utils/string/string.h"
#include "../logger.h"
#include "../mem/allocationTable.h"
#define ERROR_FILE_EXISTS -1
#define FS_ID 5

struct fileDescriptor {
	unsigned int length;
	struct fileDescriptor* parent;
	char* data;
};

struct fileRef {
	unsigned int nameLength;
	char* name;
	struct fileDescriptor* address;
};

/* inode of the current fileDescriptor */
struct fileDescriptor * currentFD;

/* A directory is just a list of (childname ; address) */
struct linkedList * currentDirectory; 

int ls(void)
{
	struct cell* cell = getIndex(currentDirectory, 0);
	int i;
	for(i = 0 ; i < size(currentDirectory) ; i ++)
	{
		struct fileRef* file = (struct fileRef*) cell->element;
		LOGA("%s\t%p\n", file->name, file->address);
		cell = cell->next;
	}
	return 0;
}

int cat(char* name, unsigned int nameLength)
{
	LOGA("%s", fdFromName(name, nameLength)->data);
	return 0;
}

int cd(char* name, unsigned int nameLength)
{
/* TODO */
}

/* Construct a directory list from file datas */
void* directoryFromFile(char* name, unsigned int nameLength)
{
	struct fileDescriptor* fd = open(name, nameLength);
	struct linkedList* newDirectory = newList();
	unsigned int size = fd->length;
	if(size%(sizeof(struct fileRef)) != 0)
	{
		LOG("Filesize is not multiple of sizeof(struct fileRef), file may be corrupted.");
		goto error;
	}

	for(; size > 0 ; size -= sizeof(struct fileRef))
	{
		struct fileRef* newFileRef = 
			allocateMemory(sizeof(struct fileRef), FS_ID);

		if (sizeof(struct fileRef) 
				!= read(fd, newFileRef, sizeof(struct fileRef)))
		{
			LOG("Error when constructing directory tree from file, bad size : aborting");
			goto error;
		}
		insert(newDirectory, newFileRef);
	}
	return newDirectory;



error:
	close(fd);
/*	while(size(newDirectory))
	{
TODO : should delete all the elements of the list
	}*/ 
	freeList(newDirectory);
	return 0;
}

int close(struct fileDescriptor* toClose)
{
	freeMemory(toClose);
}

struct fileDescriptor* open(char* name, unsigned int nameLength)
{
	struct fileDescriptor* toReturn = 
		allocateMemory(sizeof(struct fileDescriptor), FS_ID);
	struct fileDescriptor* toCopy = fdFromName(name, nameLength);
	memcpy(toCopy, toReturn, sizeof(struct fileDescriptor));
	return toReturn;
}

unsigned int read(struct fileDescriptor* fd, char* buffer, unsigned int count)
{
	if(length < count) count = length;
	unsigned int read = memcpy(&(fd->data), buffer, count);
	fd->data += read;
	fd->length -= read;
	return read;
}

struct fileDescriptor* fdFromName(char* name, unsigned int nameLength)
{
	struct cell* cell = getIndex(currentDirectory, 0);
	int i;
	for(i = 0 ; i < size(currentDirectory) ; i++)
	{
		struct fileRef* file = (struct fileRef*) cell->element;
		if(file->nameLength == nameLength 
				&& stringCmp(&nameLength, 
					&(file->name), 
					nameLength))
		{
			return file->address;
		}
	}
	return -1;
}

void* writeCurrentDirectory(void)
{

	unsigned int numberOfFile = size(currentDirectory);
	unsigned int newLength = sizeof(struct fileRef) * numberOfFile
			+ sizeof(unsigned int)
			+ sizeof(void*);

	char* newData;
	/* Write new data, i.e. a representation of the list currentDirectory */
	if(numberOfFile > 0) {
		newData = (char*) allocateMemory(
				numberOfFile * sizeof(struct fileRef), 
				FS_ID);
		char* index = newData;

		struct cell* tmp = getIndex(currentDirectory, 0);

		{
			int i;
			for(i = 0 ; i < numberOfFile ; i++)
			{
				memcpy(tmp->element, index, 
						sizeof(struct fileRef));
				index += sizeof(struct fileRef);
				tmp = tmp->next;
			}
		}

	} else {
		newData = 0;
	}


	/* Update the current fileDescriptor to that updated representation*/
	char* oldData = currentFD->data;

	currentFD->length = newLength;
	currentFD->data = newData;

	freeMemory(oldData, FS_ID);

	return;
}

int touch(char* name, unsigned int nameLength)
{
	if(size(currentDirectory) > 0){
		struct cell* tmp = getIndex(currentDirectory, 0);
		{
			int i;
			for(i = 0 ; i < size(currentDirectory) ; i++)
			{
				struct fileRef* file = 
					((struct fileRef*) tmp->element);

				if(stringCmp(file->name, name, 
							file->nameLength))
				{
					LOG("The file already exists.");
					return ERROR_FILE_EXISTS;
				}
				tmp = tmp->next;
			}
		}
	}

	struct fileDescriptor* newFile = 
		allocateMemory(sizeof(struct fileDescriptor), FS_ID);
	newFile->length = 0;
	newFile->parent = currentFD;
	newFile->data = 0;


	/* Prepare name :*/
	char* newName = allocateMemory(nameLength * sizeof(char), FS_ID);
	memcpy(name, newName, nameLength);

	struct fileRef* newFileRef = 
		allocateMemory(sizeof(struct fileRef), FS_ID);
	newFileRef->nameLength = nameLength;
	newFileRef->name = newName;
	newFileRef->address = newFile;

	insertAtEnd(currentDirectory, newFileRef);
	writeCurrentDirectory();
	return 0;
}

int mkdir(char* name, unsigned int nameLength)
{
	return touch(name, nameLength);
}

void initFS(void)
{	
	struct fileDescriptor* rootFD = 
		allocateMemory(sizeof(struct fileDescriptor), FS_ID);
	rootFD->length = 0;
	rootFD->parent = rootFD;
	rootFD->data = 0;
	currentDirectory = newList();
	currentFD = rootFD;
}

#ifdef DEBUG
#ifdef TEST_FS
void main(void)
{
	initFreeSpace();
	initManager();
	initFS();
	touch("fichier1", 9);
	mkdir("dossier1", 9);
	ls();
//	cat();
}
#endif
#endif
