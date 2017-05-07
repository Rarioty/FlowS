#include <stdio.h>

void isr_default_int(void)
{
    printf ("interrupt\n");
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
        printf("clock\n");
    }
}

void isr_kbd_int(void)
{
    printf("keyboard\n");
}
