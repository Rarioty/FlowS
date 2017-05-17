#include <kernel/devices.h>

#include <kernel/utils/registers.h>
#include <kernel/interrupts/idt.h>
#include <kernel/interrupts/irq.h>
#include <kernel/memory/ports.h>
#include <kernel/memory/io.h>
#include <kernel/tty.h>
#include <stdio.h>

unsigned char kbdmap[];

void keyboard_handler(irq_registers* r)
{
    unsigned char i;
    static int lshift_enable;
    static int rshift_enable;
    static int alt_enable;
    static int ctrl_enable;

    (void) r;

    do {
        i = inb(PORT_8042_STATUS_REGISTER);
    } while ((i & 0x01) == 0);
    i = inb(PORT_8042_DATA);
    i--;

    if (i < 0x80)
    {
        terminal_special("Key pressed: ", TERMINAL_INFO);
        printf("%02X (", i);
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

void kbd_install()
{
    terminal_special("Initializing PS/2 keyboard driver\n", TERMINAL_INFO);

    irq_install_handler(KEYBOARD_IRQ, keyboard_handler);
}

/**
 * \brief Map of a french keyboard
 */
unsigned char kbdmap[] = {
    0x1B, 0x1B, 0x1B, 0x1B,     /* Escape       */
     '&',  '1',  ' ',  ' ',
    0x82,  '2',  ' ',  ' ',
     '"',  '3',  ' ',  ' ',
    '\'',  '4',  ' ',  ' ',
     '(',  '5',  ' ',  ' ',
     '-',  '6',  ' ',  ' ',
    0x8A,  '7',  ' ',  ' ',
     '_',  '8',  ' ',  ' ',
    0x87,  '9',  ' ',  ' ',
    0x85,  '0',  ' ',  ' ',
     ')', 0xA7,  ' ',  ' ',
     '=',  '+',  ' ',  ' ',
    0x08, 0x08, 0x08, 0x08,     /* Backspace    */
    0x09, 0x09, 0x09, 0x09,     /* Tab          */
     'a',  'A',  ' ',  ' ',
     'z',  'Z',  ' ',  ' ',
     'e',  'e',  ' ',  ' ',
     'r',  'R',  ' ',  ' ',
     't',  'T',  ' ',  ' ',
     'y',  'Y',  ' ',  ' ',
     'u',  'U',  ' ',  ' ',
     'i',  'I',  ' ',  ' ',
     'o',  'O',  ' ',  ' ',
     'p',  'P',  ' ',  ' ',
     '^', 0xF9,  ' ',  ' ',
     '$', 0x9C,  ' ',  ' ',
    0x0A, 0x0A, 0x0A, 0x0A,     /* Return       */
    0xFF, 0xFF, 0xFF, 0xFF,     /* Control      */
     'q',  'Q',  ' ',  ' ',
     's',  'S',  ' ',  ' ',
     'd',  'D',  ' ',  ' ',
     'f',  'F',  ' ',  ' ',
     'g',  'G',  ' ',  ' ',
     'h',  'H',  ' ',  ' ',
     'j',  'J',  ' ',  ' ',
     'k',  'K',  ' ',  ' ',
     'l',  'L',  ' ',  ' ',
     'm',  'M',  ' ',  ' ',
    0x97,  '%',  ' ',  ' ',
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x28         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* Shift        */
     '*', 0xE6,  ' ',  ' ',
     'w',  'W',  ' ',  ' ',
     'x',  'X',  ' ',  ' ',
     'c',  'C',  ' ',  ' ',
     'v',  'V',  ' ',  ' ',
     'b',  'B',  ' ',  ' ',
     'n',  'N',  ' ',  ' ',
     ',',  '?',  ' ',  ' ',
     ';',  '.',  ' ',  ' ',
     ':',  '/',  ' ',  ' ',
     '!', 0xF5,  ' ',  ' ',
    0xFF, 0xFF, 0xFF, 0xFF,     /* R-Shift      */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x36         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* Alt          */
     ' ',  ' ',  ' ',  ' ',
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x39 (Maj)   */
    0xFF, 0xFF, 0xFF, 0xFF,     /* F1           */
    0xFF, 0xFF, 0xFF, 0xFF,     /* F2           */
    0xFF, 0xFF, 0xFF, 0xFF,     /* F3           */
    0xFF, 0xFF, 0xFF, 0xFF,     /* F4           */
    0xFF, 0xFF, 0xFF, 0xFF,     /* F5           */
    0xFF, 0xFF, 0xFF, 0xFF,     /* F6           */
    0xFF, 0xFF, 0xFF, 0xFF,     /* F7           */
    0xFF, 0xFF, 0xFF, 0xFF,     /* F8           */
    0xFF, 0xFF, 0xFF, 0xFF,     /* F9           */
    0xFF, 0xFF, 0xFF, 0xFF,     /* F10          */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x44         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x45         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x46         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x47         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x48         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x49         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x4A         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x4B         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x4C         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x4D         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x4E         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x4F         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x50         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x51         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x52         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x53         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x54         */
     '<',  '>', 0xFF, 0xFF,     /* 0x55         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x56         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x57         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x58         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x59         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x5A         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x5B         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x5C         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x5D         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x5E         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x5F         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x60         */
    0xFF, 0xFF, 0xFF, 0xFF,     /* 0x61         */
};
