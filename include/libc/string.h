#ifndef _STRING_H_
#define _STRING_H_

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EOF '\n'

size_t strlen(const char*);
void* memcpy(void* restrict, const void*, size_t);
int memcmp(const void*, const void*, size_t);
void* memset(void*, int, size_t);

#ifdef __cplusplus
}
#endif

#endif
