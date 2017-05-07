#if !defined(__i386__)
#error "You need a ix86-elf compiler !"
#endif

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

    printf("Initializing IDT...\n");
    init_idt();

    printf("Initializing PIC...\n");
    init_pic();

    printf("Initializing GDT...\n");
    init_gdt();

    asm("movw $0x18, %ax \n \
         movw %ax, %ss   \n \
         movl $0x20000, %esp");

    main();
}

int main(void)
{
    printf("kernel: GDT loaded !\n");

    sti;

    printf("kernel: Interrupts are allowed\n");

    while(1);

    return 1;
}
