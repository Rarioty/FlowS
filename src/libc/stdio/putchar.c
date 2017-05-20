#include <stdio.h>

#if defined(__is_libk)
#include <kernel/tty.h>
#endif

int putchar(int ic) {
#if defined(__is_libk)
    char c = (char) ic;
    terminal_write(&c, sizeof(c));
#else
    // Do not forget to implement stdio and the write system call.
    #error "Implement me before goind to userland !"
#endif

    return ic;
}
