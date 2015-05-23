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

#define LOGA(text, ...) print("[log] "text"\n")
#define LOG_INT(i) print("[log] "); printInt(i); print("\n");

#endif

#define LOG(text) LOGA(text, "")
