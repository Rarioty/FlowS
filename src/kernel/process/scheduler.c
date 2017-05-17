#include <kernel/process/scheduler.h>

#include <kernel/process/process.h>
#include <kernel/memory/gdt.h>
#include <stddef.h>
#include <stdio.h>

void schedule(irq_registers* r)
{
    uint32_t esp0, eflags;
    uint16_t ss, cs;

    // If there is a ready process and no one is loaded, launch it !
    if (current == 0 && n_proc)
        current = &p_list[0];
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

        current->regs.esp       = r->useresp;
        current->regs.ss        = r->ss;

        // Clean the stack before changing stack to wipe all registers pushed
        default_tss.esp0 = (uint32_t) (r+1);

        // Simple round-robin for new process
        if (n_proc > current->pid + 1)
            current = &p_list[current->pid+1];
        else
            current = &p_list[0];
    }

    // Switch the task now
    ss      = current->regs.ss;
    cs      = current->regs.cs;
    eflags  = (current->regs.eflags | 0x200) & 0xFFFFBFFF;
    esp0    = default_tss.esp0;

    asm("mov  %0, %%es          \n  \
         push %1                \n  \
         push %2                \n  \
         push %3                \n  \
         push %4                \n  \
         push %5                \n  \
         push %6                \n  \
         ljmp $0x08, $do_switch \n"
         :: "m" (esp0), "m" (ss), "m" (current->regs.esp), "m" (eflags), "m" (cs), "m" (current->regs.eip), "m" (current));
}
