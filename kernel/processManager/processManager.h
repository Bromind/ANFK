struct cell * createProcess(void (*f)(void), void* baseAddress);
struct cell * createEmptyProcess(void);
void deleteProcess();
void switchToProcess(void* process);
struct processDescriptor * getCurrentProcess(void);
unsigned int getPID(void);
void printProcess(struct processDescriptor* process);

void start(struct cell * processCell);
void stop(struct cell * processCell);
void yield(void);
