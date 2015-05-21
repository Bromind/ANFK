#ifndef LINKEDLIST_H
#include "../../utils/linkedList.h"
#define LINKEDLIST_H
#endif

#ifndef STRING_H
#include "../../utils/string/string.h"
#define STRING_H
#endif

#ifndef LOGGER_H
#include "../logger.h"
#define LOGGER_H
#endif

#ifndef ALLOCATION_TABLE_H
#include "../mem/allocationTable.h"
#define ALLOCATION_TABLE_H
#endif 


#define ERROR_FILE_EXISTS -1
#define FS_ID 5

#ifndef FS_H
#include "fs.h"
#define FS_H
#endif


/* Internal functions */
struct fileDescriptor* fdFromName(char* name, unsigned int nameLength);
struct linkedList* directoryFromFile(char* name, unsigned int nameLength);

/* inode of the current fileDescriptor */
struct fileDescriptor * currentFD;

/* A directory is just a list of (childname ; address) */
struct linkedList * currentDirectory; 

int ls(void)
{
	LOGA("Directory : %s\n", currentFD->selfRef->name);
	struct cell* cell = getIndex(currentDirectory, 0);
	int i;
	for(i = 0 ; i < size(currentDirectory) ; i ++)
	{
		struct fileRef* file = (struct fileRef*) cell->element;
		LOGA("%s\t%p\n", file->name, file->address);
		cell = cell->next;
	}
	LOG("\n\n\n");
	return 0;
}

int cat(char* name, unsigned int nameLength)
{
	LOGA("File : %s\n", name);
	struct fileDescriptor* toCat = fdFromName(name, nameLength);
	LOGA("%s\n\n\n", toCat->data);
	return 0;
}

int cd(char* name, unsigned int nameLength)
{
	struct fileDescriptor* newFD = open(name, nameLength);
	struct linkedList* newDirectory = directoryFromFile(name, nameLength);
	struct fileDescriptor* oldFD = currentFD;
	struct linkedList* oldDirectory = currentDirectory;
	currentFD = newFD;
	currentDirectory = newDirectory;
	close(oldFD);
	while(size(oldDirectory))
	{
		void* elem = removeCell(oldDirectory, 
				getIndex(oldDirectory, 0));
			if(elem)
				kfree(elem);
	}
	freeList(oldDirectory);
	return 0;
}

/* Construct a directory list from file datas */
struct linkedList* directoryFromFile(char* name, unsigned int nameLength)
{
	struct fileDescriptor* fd = open(name, nameLength);
	
	
	unsigned int numberOfFile = 0;
	if(sizeof(unsigned int) != read(fd, (char*) &numberOfFile, 
				sizeof(unsigned int)))
	{
		LOG("Can't read number of files, aborting");
		goto error;
	}


	struct linkedList* newDirectory = newList();

	int i = 0;
	for(; i < numberOfFile ; i++)
	{
		struct fileRef* newFileRef = 
			kalloc(sizeof(struct fileRef));

		if (sizeof(struct fileRef) != read(fd,(char*) newFileRef, 
					sizeof(struct fileRef)))
		{
			LOG("Error when constructing directory tree from file, can't read : aborting");
			goto error;
		}
		unsigned int nameLength = newFileRef->nameLength;
		newFileRef->name = kalloc(sizeof(char)*nameLength);
		if(nameLength * sizeof(char) 
				!= read(fd, newFileRef->name, nameLength))
		{
			LOG("Error when constructing directory tree from file, can't read : aborting");
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
	kfree(toClose);
	return 0;
}

struct fileDescriptor* open(char* name, unsigned int nameLength)
{
	struct fileDescriptor* toReturn = 
		kalloc(sizeof(struct fileDescriptor));
	struct fileDescriptor* toCopy = fdFromName(name, nameLength);
	memcpy(toCopy, toReturn, sizeof(struct fileDescriptor));
	return toReturn;
}

unsigned int read(struct fileDescriptor* fd, char* buffer, unsigned int count)
{
	if(fd->length < count) count = fd->length;
	unsigned int read = memcpy(fd->data, buffer, count);
	fd->data += read;
	fd->length -= read;
	return read;
}

unsigned int write(struct fileDescriptor* fd, char* buffer, unsigned int count)
{
	unsigned int written;
	/* if the size of the file increase, re-write all the file */
	if(count > fd->length)
	{
		struct fileDescriptor* self = fd->selfRef->address;
		char* newData = 
			kalloc(self->length + count - fd->length);
		memcpy(self->data, newData, self->length - fd->length);
		void* offset = newData+self->length - fd->length;
		memcpy(buffer, offset, count);
		self->length += count - fd->length;
		char* oldData = self->data;
		self->data = newData;
		fd->length = 0;
		fd->data = self->data + self->length;
		kfree(oldData);
		written = count;
	} else {
		memcpy(buffer, fd->data, count);
		fd->length -= count;
		fd->data += count;
		written = count;
	}
	return written;
}

struct fileDescriptor* fdFromName(char* name, unsigned int nameLength)
{
	if(nameLength == 2 && stringCmp(name, "..", 2))
	{
		return currentFD->parent;
	}
	if(nameLength == 1 && stringCmp(name, ".", 1))
	{
		return currentFD;
	}
	struct cell* cell = getIndex(currentDirectory, 0);
	int i;
	for(i = 0 ; i < size(currentDirectory) ; i++)
	{
		struct fileRef* file = (struct fileRef*) cell->element;
		if(file->nameLength == nameLength 
				&& stringCmp(name, 
					file->name, 
					nameLength))
		{
			return file->address;
		}
		cell = cell->next;
	}
	return 0;
}

/* Return a ref to the new data */
void* writeCurrentDirectory(void)
{
	unsigned int numberOfFile = size(currentDirectory);
	unsigned int subfileNamesLength = 0;

	/* Computing names of files*/
	{
		int i;
		for(i = 0; i < numberOfFile ; i++)
		{
			subfileNamesLength += 
				((struct fileRef*)
				 getIndex(currentDirectory, i)->element)
				->nameLength;
		}
	}

	unsigned int newLength = 
		+ sizeof(unsigned int) /* number of subfiles*/
		+ (sizeof(struct fileRef)) * numberOfFile /*filerefs content */
		+ sizeof(char) * subfileNamesLength; /* Save subfile names */

	char* newData;
	/* Write new data, i.e. a representation of the list currentDirectory */
	if(numberOfFile > 0) {
		newData = (char*) kalloc(newLength);
		char* index = newData;
		memcpy(&numberOfFile, index, sizeof(unsigned int));
		index += sizeof(unsigned int);

		struct cell* tmp = getIndex(currentDirectory, 0);
		{
			int i;
			for(i = 0 ; i < numberOfFile ; i++)
			{
				struct fileRef* ref =
					(struct fileRef*)tmp->element;

				memcpy(ref, index, sizeof(struct fileRef));
				index += sizeof(struct fileRef);
				
				unsigned int nameLength = ref->nameLength;

				memcpy(ref->name, index, nameLength);
				index += nameLength;
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

	kfree(oldData);

	return newData;
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
		kalloc(sizeof(struct fileDescriptor));
	newFile->length = 0;
	newFile->parent = currentFD;
	newFile->selfRef = kalloc(sizeof(struct fileRef));
	newFile->data = 0;


	/* Prepare name :*/
	char* newName = kalloc(nameLength * sizeof(char));
	memcpy(name, newName, nameLength);

	newFile->selfRef->nameLength = nameLength;
	newFile->selfRef->name = newName;
	newFile->selfRef->address = newFile;

	insertAtEnd(currentDirectory, newFile->selfRef);

	writeCurrentDirectory();
	return 0;
}


/* touch file & write it once to have the header*/
int mkdir(char* name, unsigned int nameLength)
{
	unsigned int defaultDirContent = 0;
	touch(name, nameLength);
	struct fileDescriptor* fd = open(name, nameLength);
	write(fd, (char*)&defaultDirContent, sizeof(unsigned int));
	return 0;
}

void initFS(void)
{	
	struct fileDescriptor* rootFD = 
		kalloc(sizeof(struct fileDescriptor));
	rootFD->length = 0;
	rootFD->parent = rootFD;
	rootFD->selfRef = kalloc(sizeof(struct fileRef));
	rootFD->data = 0;

	rootFD->selfRef->nameLength = 5;
	rootFD->selfRef->name = kalloc(5);
	memcpy("root", rootFD->selfRef->name, 5);
	rootFD->selfRef->address = rootFD;
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
	struct fileDescriptor* f1 = open("fichier1", 9);
	write(f1, "Banane", 6);
	close(f1);
	cat("fichier1", 9);
	ls();
	cd("dossier1", 9);
	ls();
	cd("..", 2);
	ls();
	cat("fichier1", 9);
}
#endif
#endif
