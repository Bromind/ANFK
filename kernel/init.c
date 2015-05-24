#ifndef LOOGER_H
#include "logger.h"
#define LOOGER_H
#endif 

#ifndef SYSCALL_H
#include "syscall/syscall.h"
#define SYSCALL_H
#endif

void init(void)
{
	LOG("Running f2\n");
	int instr = 0xe1a0f00e; /* mov pc, lr*/
	sys_touch("test\0");
	struct fileDescriptor* fd = sys_open("test\0");

	sys_write(fd,(char*) &instr, sizeof(int));

	sys_close(fd);
	LOG("exec");
	sys_exec("test", "");
	sys_cat("test\0");
	LOG("Exit");
}

