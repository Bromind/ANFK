#ifndef FS_H
#include "../fs/fs.h"
#define FS_H
#endif

/*Null terminating fileName*/
struct fileDescriptor* sys_open(char* name)
{
	unsigned int nameLength = 0;
	while(*(name+nameLength)) nameLength++;
	return open(name, nameLength);
}

int sys_close(struct fileDescriptor* fd)
{
	return close(fd);
}

int sys_touch(char* name)
{
	unsigned int nameLength = 0;
	while(*(name+nameLength)) nameLength++;
	return touch(name, nameLength);
}

int sys_mkdir(char* name)
{
	unsigned int nameLength = 0;
	while(*(name+nameLength)) nameLength++;
	return mkdir(name, nameLength);
}

unsigned int sys_read(struct fileDescriptor* fd, char* buffer, 
		unsigned int count){
	return read(fd, buffer, count);
}

unsigned int sys_write(struct fileDescriptor* fd, char* buffer, 
		unsigned int count)
{
	return write(fd, buffer, count);
}


int sys_ls(void)
{
	return ls();
}
int sys_cat(char* name)
{
	unsigned int nameLength = 0;
	while(*(name+nameLength)) nameLength++;
	return cat(name, nameLength);
}
int sys_cd(char* name)
{
	unsigned int nameLength = 0;
	while(*(name+nameLength)) nameLength++;
	return cd(name, nameLength);
}
