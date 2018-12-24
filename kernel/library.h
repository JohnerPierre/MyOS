#include "../common/types.h"
#include "screen.h"
#include "cursor.h"

void *memset(void *dst, int value, uint count);

void *memcpy(void *dst, void *src, uint count);

int strncmp(const char *p, const char *q, uint n);

void itos(int value, char* result, int b);

void print(char*s, ...);

int strlen( char *testStr );