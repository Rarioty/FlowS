#if !defined(__i386__)
#error "You need a ix86-elf compiler !"
#endif

#include <kernel/cursor.h>
#include <kernel/devices.h>
#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/irq.h>
#include <kernel/pic.h>
#include <kernel/io.h>
#include <string.h>
#include <stdio.h>

int main(void);

void kernel_main(void)
{
    terminal_initialize();
    terminal_writeline("=== FlowS ===");

    hide_cursor();

    terminal_special("kernel: Clearing interrupts !\n", TERMINAL_WARNING);
    cli;

    terminal_special("kernel: Initializing IDT...\n", TERMINAL_INFO);
    init_idt();

    terminal_special("kernel: Initializing PIC...\n", TERMINAL_INFO);
    init_pic();

    terminal_special("kernel: Initializing GDT...\n", TERMINAL_INFO);
    init_gdt();

    // Initialize the tss
    asm("movw $0x38, %ax    \n \
         ltr %ax");
    terminal_special("kernel: TR loaded\n", TERMINAL_INFO);

    asm("movw $0x18, %ax    \n \
         movw %ax, %ss      \n \
         movl $0x20000, %esp");

    main();
}

void task1(void)
{
    while(1);

    return;
}

int main(void)
{
    terminal_special("kernel: GDT loaded !\n", TERMINAL_INFO);

    // Install devices
    terminal_special("kernel: Installing devices...\n", TERMINAL_INFO);
    kbd_install();
    clock_install();

    sti;
    terminal_special("kernel: Interrupts are allowed\n", TERMINAL_WARNING);

    // Copy the function to the correct address
    memcpy((char*)0x30000, &task1, 100);

    // Switch to ring 3 and jump to task1
    terminal_special("kernel: Switching to ring 3 !\n", TERMINAL_WARNING);
    asm("cli                        \n \
         push $0x33                 \n \
         push $0x30000              \n \
         pushfl                     \n \
         popl %%eax                 \n \
         orl $0x200, %%eax          \n \
         and $0xFFFFBFFF, %%eax     \n \
         push %%eax                 \n \
         push $0x23                 \n \
         push $0x00                 \n \
         movl $0x20000, %0          \n \
         movw $0x2B, %%ax           \n \
         movw %%ax, %%ds            \n \
         iret" : "=m"(default_tss.esp0):);

    // Never reached !
    terminal_special("This should never be reached !\n", TERMINAL_ERROR);
    asm("hlt");

    return 1;
}
