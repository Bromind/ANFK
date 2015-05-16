#ifndef STRUCT_PROCESSSTATE_H
#include "processState.h"
#define STRUCT_PROCESSSTATE_H
#endif

extern void transfer(struct processState* nextProcess, struct processState* runningProcess);
extern void startProcess(struct processState* process);
extern void restartProcess(struct processState* process);
