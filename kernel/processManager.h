#define NBR_REGISTER 15
#define REGISTER_SIZE 4

void* createProcess(void (*f)(void), void* stack, int stackSize);
void switchToProcess(void* process);
