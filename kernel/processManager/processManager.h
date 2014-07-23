#include "../../utils/linkedList.h"

#define NBR_REGISTER 15
#define REGISTER_SIZE 4

struct cell * createProcess(void (*f)(void), void* stack, int stackSize);
void deleteProcess();
void switchToProcess(void* process);

void start(struct cell * processCell);
void stop(struct cell * processCell);
void yield(void);
void initManager(void);
void startKernel(void);
