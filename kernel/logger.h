#define LOGA(text, ...) printf("[log] "text"\n", __VA_ARGS__)
#define LOG(text) LOGA(text, "")
