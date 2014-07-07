#include <stdlib.h>
#include <stdio.h>
#include "processManager.h"

void hello(void)
{
	printf("Hello, World !\n");
}

void goodBye(void)
{
	printf("Goodbye, World !\n");
}

int main(void)
{
	void* stack1 = malloc(200);
	void* stack2 = malloc(200);
	void* process1 = createProcess(&hello, stack1, 16);
	void* process2 = createProcess(&goodBye, stack2, 16);
	switchToProcess(process2);
}

