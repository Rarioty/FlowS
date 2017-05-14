#include <kernel/ports.h>
#include <kernel/tty.h>
#include <kernel/kbd.h>
#include <kernel/io.h>
#include <stdio.h>

void isr_default_int(void)
{
    terminal_writeline("interrupt");
}

void isr_GP_exc(void)
{
    terminal_writeline("GP fault !");
    while(1);
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
        putchar('.');
    }
}

void isr_kbd_int(void)
{
    unsigned char i;
    static int lshift_enable;
    static int rshift_enable;
    static int alt_enable;
    static int ctrl_enable;

    do {
        i = inb(PORT_8042_STATUS_REGISTER);
    } while ((i & 0x01) == 0);
    i = inb(PORT_8042_DATA);
    i--;

    if (i < 0x80)
    {
        terminal_special("Key pressed: ", TERMINAL_INFO);
        printf("%x (", i);
        if (lshift_enable || rshift_enable)
            printf("1 - ");
        else
            printf("0 - ");

        if (alt_enable)
            printf("1 - ");
        else
            printf("0 - ");

        if (ctrl_enable)
            printf("1) --> ");
        else
            printf("0) --> ");

        switch(i)
        {
            case 0x29:
                lshift_enable = 1;
                break;
            case 0x35:
                rshift_enable = 1;
                break;
            case 0x1C:
                ctrl_enable = 1;
                break;
            case 0x37:
                alt_enable = 1;
                break;
            default:
                putchar(kbdmap[i * 4 + (lshift_enable || rshift_enable) + ctrl_enable + alt_enable]);
                break;
        }

        printf("\n");
    }
    else
    {
        i -= 0x80;
        switch(i)
        {
            case 0x29:
                lshift_enable = 0;
                break;
            case 0x35:
                rshift_enable = 0;
                break;
            case 0x1C:
                ctrl_enable = 0;
                break;
            case 0x37:
                alt_enable = 0;
                break;
        }
    }
}
