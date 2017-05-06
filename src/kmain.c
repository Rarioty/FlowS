#if !defined(__i386__)
#error "You need a ix86-elf compiler !"
#endif

#include <kernel/tty.h>

int kernel_main(void)
{
    terminal_initialize();
    terminal_putchar('0');
    terminal_putchar('b');

    return 1;
}
