ENTRY(_start)                   /* the name of the entry label */

SECTIONS {
    . = 0x00100000;             /* the code should be loaded at 1MB */

    .multiboot :
    {
        *(.multiboot)           /* place multiboot header FIRST ! */
    }

    .text ALIGN (0x1000) :      /* align at 4 KB */
    {
        *(.text)                /* all text sections from all files */
        *(.rodata)              /* all read-only data sections from all files */
    }

    .data ALIGN (0x1000) :      /* align at 4 KB */
    {
        *(.data)                /* all data sections from all files */
    }

    .bss ALIGN (0x1000) :       /* align at 4 KB */
    {
        *(COMMON)               /* all COMMON sections from all files */
        *(.bss)                 /* all bss sections from all files */
    }
}
