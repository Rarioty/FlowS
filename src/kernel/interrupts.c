#include <kernel/tty.h>

void isr_default_int(void)
{
    terminal_writeline("interrupt");
}

void isr_clock_int(void)
{
    static int tic = 0;
    static int sec = 0;
    tic++;
    if (tic % 100 == 0)
    {
        sec++;
        tic = 0;
        terminal_writeline("clock");
    }
}

void isr_kbd_int(void)
{
    terminal_writeline("keyboard");
}
