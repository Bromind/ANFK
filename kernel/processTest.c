#include <stdlib.h>
#include <stdio.h>
#include "processManager.h"

void hello(void)
{
	printf("Hello, World !\n");
}

int main(void)
{
	initManager();

	void* stack1 = malloc(200);
	void* process1 = createProcess(&hello, stack1, 16);
	start(process1);
	startKernel();
}

