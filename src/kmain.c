#if !defined(__i386__)
#error "You need a ix86-elf compiler !"
#endif

#include <kernel/syscalls.h>
#include <kernel/process.h>
#include <kernel/devices.h>
#include <kernel/cursor.h>
#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/irq.h>
#include <kernel/pic.h>
#include <kernel/mm.h>
#include <kernel/io.h>
#include <string.h>
#include <stdio.h>

int main(void);

void kernel_main(void)
{
    terminal_initialize();
    terminal_writeline("          === FlowS ===");

    hide_cursor();

    terminal_special("kernel: Clearing interrupts !\n", TERMINAL_WARNING);
    cli;

    terminal_special("kernel: Initializing GDT...\n", TERMINAL_INFO);
    init_gdt();

    asm("movw $0x18, %ax    \n \
         movw %ax, %ss      \n \
         movl $0x20000, %esp");

    main();
}

void task1(void)
{
    char *msg = (char*) 0x40000100; /* le message sera stocké en 0x30100 */
    int i;

    msg[0] = 't';
    msg[1] = 'a';
    msg[2] = 's';
    msg[3] = 'k';
    msg[4] = '1';
    msg[5] = '\n';
    msg[6] = 0;

    while (1)
    {
        asm("mov %0, %%ebx; mov $0x00, %%eax; int $0x30" :: "m" (msg));
        for (i = 0; i < 1000000; ++i);
    }

    return;
}

void task2(void)
{
    char *msg = (char*) 0x40000100; /* le message sera stocké en 0x30100 */
    int i;

    msg[0] = 't';
    msg[1] = 'a';
    msg[2] = 's';
    msg[3] = 'k';
    msg[4] = '2';
    msg[5] = '\n';
    msg[6] = 0;

    while (1)
    {
        asm("mov %0, %%ebx; mov $0x00, %%eax; int $0x30" :: "m" (msg));
        for (i = 0; i < 1000000; ++i);
    }

    return;
}

int main(void)
{
    terminal_special("kernel: GDT loaded !\n", TERMINAL_INFO);

    init_idt();
    terminal_special("kernel: IDT initialized\n", TERMINAL_INFO);

    terminal_special("kernel: PIC initialized\n", TERMINAL_INFO);
    init_pic();

    // Initialize the tss
    asm("movw $0x38, %ax    \n \
         ltr %ax");
    terminal_special("kernel: TR loaded\n", TERMINAL_INFO);

    init_mm();
    terminal_special("kernel: Paging enabled\n", TERMINAL_WARNING);

    // Install hardware devices
    terminal_special("kernel: Installing devices...\n", TERMINAL_INFO);
    syscalls_install();
    kbd_install();
    clock_install();
    terminal_special("kernel: Devices installed\n", TERMINAL_INFO);

    terminal_special("kernel: Creating tasks...", TERMINAL_WARNING);

    // Load tasks
    load_task((uint32_t*) 0x200000, (uint32_t*) &task1, 0x2000);
    load_task((uint32_t*) 0x300000, (uint32_t*) &task2, 0x2000);

    terminal_special("DONE\n", TERMINAL_WARNING);

    terminal_special("kernel: Setting interrupts !\n", TERMINAL_WARNING);
    sti;

    while (1);

    return 1;
}
