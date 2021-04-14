#ifndef DEBUG_HEADER
#define DEBUG_HEADER

#include <stdio.h>

#ifdef DEBUG
#define debug(fmt, ...) do { \
	fprintf(stderr, "[ debug ] (%s:%d): " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
} while (0)
#else
#define debug(fmt, ...)
#endif

#endif