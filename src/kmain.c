#if !defined(__i386__)
#error "You need a ix86-elf compiler !"
#endif

#include <string.h>

int kernel_main(void)
{
    return strlen("test\0");
}
