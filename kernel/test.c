#ifndef PROCESS_MANAGER_H
#include "processManager/processManager.h"
#define PROCESS_MANAGER_H
#endif 

#ifndef LOOGER_H
#include "logger.h"
#define LOOGER_H
#endif 

#include "syscall/syscall.h"

#ifndef INIT_SUBSYSTEMS_H
#include "initSubsystems.h"
#define INIT_SUBSYSTEMS_H
#endif

#define MAIN_ID 3
/*
void f1(void)
{
	unsigned int pid;
	do{
		pid = getPID();
		LOG("p1"); 
	}while(sys_fork() == pid);
	return;
}
*/
void f2(void)
{
	int instr1 = 0xe52db004;
	int instr2 = 0xe28db000;
	int instr3 = 0xe28bd000;
	int instr4 = 0xe8bd0800;
	int instr5 = 0xe12fff1e;
	sys_touch("test");
	struct fileDescriptor* fd = sys_open("test");

	sys_write(fd,(char*) &instr1, sizeof(int));
	sys_write(fd,(char*) &instr2, sizeof(int));
	sys_write(fd,(char*) &instr3, sizeof(int));
	sys_write(fd,(char*) &instr4, sizeof(int));
	sys_write(fd,(char*) &instr5, sizeof(int));

	sys_close(fd);
	sys_exec("test", "");
	sys_cat("test");

}

void main(void)
{
	initSubsystems();
	struct cell* p1 = createProcess(&f2, 0);
	start(p1);
	startScheduler();
}
