#ifdef DEBUG

#ifndef STDIO_H
#include <stdio.h>
#define STDIO_H
#endif

#ifndef PROCESS_MANAGER_H
#include "processManager/processManager.h"
#define PROCESS_MANAGER_H
#endif

#define LOGA(text, ...) printf("[log process %i] "text"\n", getPID(), __VA_ARGS__); fflush(stdout)
#define LOG_INT(i) printf("%i", i); fflush(stdout)

#else

#include "initFrameBuffer.h"

#define LOGA(text, ...) print("[log] "); print(text)
#define LOG_INT(i) printInt(i);
#define LOG_CONT(text) print(text)

void eraseScreen(void);

#endif

#define LOG(text) LOGA(text, "")
