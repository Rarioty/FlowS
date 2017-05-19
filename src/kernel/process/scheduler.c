#include <kernel/process/scheduler.h>

#include <kernel/process/process.h>
#include <kernel/memory/gdt.h>
#include <stddef.h>
#include <stdio.h>

#define     USER_MODE       0
#define     KERNEL_MODE     1

void switch_to_task(int pid, int mode)
{
    uint32_t kernel_stack, eflags;
    uint16_t kernel_ss, ss, cs;

    current = &p_list[pid];

    // Load the tss
    default_tss.ss0     = current->kernel_stack.ss0;
    default_tss.esp0    = current->kernel_stack.esp0;

    /*
     * Stock the ss, esp, eflags, cs and eip registers which are necessary
     * to the switch. Then the do_switch function restore the registers,
     * the page table of the new active process and switch with the iret
     * instruction.
     */

    ss      = current->regs.ss;
    cs      = current->regs.cs;
    eflags  = (current->regs.eflags | 0x200) & 0xFFFFBFFF;

    if (mode == USER_MODE)
    {
        kernel_ss     = current->kernel_stack.ss0;
        kernel_stack  = current->kernel_stack.esp0;
    }
    else    // Kernel mode
    {
        kernel_ss     = current->regs.ss;
        kernel_stack  = current->regs.esp;
    }

    asm("mov  %0, %%ss          \n  \
         mov  %1, %%esp         \n  \
         cmp  %[KMODE], %[mode] \n  \
         je   next              \n  \
         push %2                \n  \
         push %3                \n  \
         next:                  \n  \
         push %4                \n  \
         push %5                \n  \
         push %6                \n  \
         push %7                \n  \
         ljmp $0x08, $do_switch \n"
         :: "m" (kernel_ss), "m" (kernel_stack), "m" (ss), "m" (current->regs.esp), "m" (eflags), "m" (cs), "m" (current->regs.eip), "m" (current), \
         [KMODE] "i" (KERNEL_MODE), [mode] "g" (mode));
}

void schedule(irq_registers* r)
{
    struct process* p;
    uint32_t i;
    int newpid;

    // If there is a ready process and no one is loaded, launch it !
    if (current == 0 && n_proc)
        switch_to_task(0, USER_MODE);
    // If there is only one process or no one, we return
    else if (n_proc <= 1)
        return;
    // If there is at least two process, we change the active one
    else if (n_proc > 1)
    {
        // Save the registers for the current task
        current->regs.eflags    = r->eflags;
        current->regs.cs        = r->cs;
        current->regs.eip       = r->eip;
        current->regs.eax       = r->eax;
        current->regs.ecx       = r->ecx;
        current->regs.edx       = r->edx;
        current->regs.ebx       = r->ebx;
        current->regs.ebp       = r->ebp;
        current->regs.esi       = r->esi;
        current->regs.edi       = r->edi;
        current->regs.ds        = r->ds;
        current->regs.es        = r->es;
        current->regs.fs        = r->fs;
        current->regs.gs        = r->gs;

        if (current->regs.cs != 0x8)
        {
            current->regs.esp   = r->useresp;
            current->regs.ss    = r->ss;
        }
        else
        {
            current->regs.esp   = r->useresp;
            current->regs.ss    = default_tss.ss0;
        }

        // Save the tss
        current->kernel_stack.ss0     = default_tss.ss0;
        current->kernel_stack.esp0    = default_tss.esp0;
    }

    // Choose new process
    newpid = 0;
    for (i = current->pid + 1; i < MAX_PID && newpid == 0; ++i)
    {
        if (p_list[i].state == 1)
            newpid = i;
    }

    if (!newpid)
    {
        for (i = 1; i < current->pid && newpid == 0; ++i)
        {
            if (p_list[i].state == 1)
                newpid = i;
        }
    }

    p = &p_list[newpid];

    // Switch now
    if (p->regs.cs != 0x8)
        switch_to_task(p->pid, USER_MODE);
    else
        switch_to_task(p->pid, KERNEL_MODE);
}
