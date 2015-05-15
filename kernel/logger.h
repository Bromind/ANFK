#ifdef DEBUG

#ifndef STDIO_H
#include <stdio.h>
#define STDIO_H
#endif

#define LOGA(text, ...) printf("[log] "text"\n", __VA_ARGS__)

#else

#define LOGA(text, ...) print("[log] "text"\n")

#endif

#define LOG(text) LOGA(text, "")
