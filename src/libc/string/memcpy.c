#include <string.h>

void* memcpy(void* dstptr, const void* srcptr, size_t size)
{
    size_t i;

    unsigned char* dst = (unsigned char*) dstptr;
    const unsigned char* src = (const unsigned char*) srcptr;
    for (i = 0; i < size; ++i)
        dst[i] = src[i];

    return dstptr;
}
