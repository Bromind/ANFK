/*Stack 1MiB */
#define STACKSIZE 1048576

#ifdef DEBUG
#include "stackPointerAccess.h"
void* originalStack;
void* stackTop;
#else 
#include "stack.h"
#endif

#ifdef DEBUG
	/* When debugging, allocate new stack */
inline void initStack(void)
{
	originalStack = getStackPointer();
	stackTop = malloc(STACKSIZE);
	stackValue = stackTop + STACKSIZE - 1;
	setStackPointer(stackValue);
}

/* Reset stack to its original value */
inline void resetStack(void)
{
	setStackPointer(originalStack);
	free(stackTop);
}

#endif
