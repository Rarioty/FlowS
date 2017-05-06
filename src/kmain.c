#if !defined(__i386__)
#error "You need a ix86-elf compiler !"
#endif

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <stdio.h>

int main(void);

void kernel_main(void)
{
    terminal_initialize();
    terminal_writeline("=== FlowS ===");

    // Check if we are in protected mode !
    unsigned int cr0;
    asm("movl %%cr0, %0\n" : "=r"(cr0));

    printf("Protected mode: ");
    if (cr0 & 1)
        printf("ON\n");
    else
    {
        printf("OFF\n");
        printf("ERROR: We should be in protected mode !\n");
        return;
    }

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

    while(1);

    return 1;
}
