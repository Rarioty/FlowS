#if !defined(__i386__)
#error "You need a ix86-elf compiler !"
#endif

#include <kernel/tty.h>
#include <stdio.h>

int kernel_main(void)
{
    terminal_initialize();
    terminal_writeline("=== FlowS ===");

    for (int i = 20; i < 70; ++i)
    {
        printf("testing scrolling (%c)\n", i);
    }

    return 1;
}
