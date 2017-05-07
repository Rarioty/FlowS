#if !defined(__i386__)
#error "You need a ix86-elf compiler !"
#endif

#include <kernel/cursor.h>
#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/io.h>
#include <stdio.h>

int main(void);

void kernel_main(void)
{
    terminal_initialize();
    terminal_writeline("=== FlowS ===");

    hide_cursor();

    terminal_special("Initializing IDT...\n", TERMINAL_INFO);
    init_idt();

    terminal_special("Initializing PIC...\n", TERMINAL_INFO);
    init_pic();

    terminal_special("Initializing GDT...\n", TERMINAL_INFO);
    init_gdt();

    asm("movw $0x18, %ax \n \
         movw %ax, %ss   \n \
         movl $0x20000, %esp");

    main();
}

int main(void)
{
    terminal_special("kernel: GDT loaded !\n", TERMINAL_INFO);

    sti;

    terminal_special("kernel: Interrupts are allowed\n", TERMINAL_WARNING);

    while(1);

    return 1;
}
