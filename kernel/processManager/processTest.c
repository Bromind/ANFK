#include <stdlib.h>
#include <stdio.h>
#include "processManager.h"
#include "initSP.h"
#include "initFrameBuffer.h"

#define SP_INIT 8000
/*
void hello(void)
{
	printf("Hello, World !\n");
	deleteProcess();
}

void bye(void)
{
	printf("Goodbye, World !\n");
	deleteProcess();
}
*/

int main(void)
{
	initSP(SP_INIT);
	initFB();
	printChar('A');
	/*
	initManager();

	void* stack1 = malloc(200);
	void* process1 = createProcess(&hello, stack1, 16);
	void* stack2 = malloc(200);
	void* process2 = createProcess(&bye, stack2, 16);
	start(process1);
	start(process2);
	startKernel();*/
}

