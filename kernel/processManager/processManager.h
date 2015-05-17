struct cell * createProcess(void (*f)(void), void* stack, int stackSize);
void deleteProcess();
void switchToProcess(void* process);
struct processDescriptor * getCurrentProcess(void);

void start(struct cell * processCell);
void stop(struct cell * processCell);
void yield(void);
void initManager(void);
void startKernel(void);
