#if !defined(__i386__)
#error "You need a ix86-elf compiler !"
#endif

#include <kernel/tty.h>
#include <stdio.h>

int kernel_main(void)
{
    terminal_initialize();
    terminal_writeline("=== FlowS ===");
    terminal_writeline("testing...");

    printf("test %c - %s", 'a', "ok");

    return 1;
}
