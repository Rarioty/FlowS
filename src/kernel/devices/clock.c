#include <kernel/devices.h>
#include <kernel/tty.h>
#include <kernel/irq.h>
#include <stdio.h>

void clock_handler(struct regs* r)
{
    (void) r;
    static int tic = 0;
    static int sec = 0;
    tic++;
    if (tic % 100 == 0)
    {
        sec++;
        tic = 0;
        putchar('.');
    }
}

void clock_install(void)
{
    terminal_special("Initializing clock driver\n", TERMINAL_INFO);

    irq_install_handler(CLOCK_IRQ, clock_handler);
}
