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

#else

#define LOGA(text, ...) print("[log] "text"\n")

#endif

#define LOG(text) LOGA(text, "")
