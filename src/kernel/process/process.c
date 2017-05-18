#include <kernel/memory/mm.h>
#include <string.h>
#include <stdio.h>

#define __PLIST__
#include <kernel/process/process.h>

void load_task(uint32_t* code_phys_addr, uint32_t* fn, unsigned int code_size)
{
    uint32_t page_base, pages, user_stack_base, i;
    uint32_t* pd;

    // Copy the code to the specified memory location
    memcpy((char*) code_phys_addr, (char*) fn, code_size);

    // Update the bitmap
    page_base = (uint32_t) PAGE(code_phys_addr);

    if (code_size % PAGE_SIZE)
        pages = code_size / PAGE_SIZE + 1;
    else
        pages = code_size / PAGE_SIZE;

    for (i = 0; i < pages; ++i)
        set_page_frame_used(page_base + i);

    // Create the page directory and page tables
    pd = pd_create(code_phys_addr, code_size);

    // Create a specific stack for the task
    user_stack_base = (uint32_t) get_page_frame();
    if (user_stack_base > 0x400000) {
        printf("Not enough memory to create kernel stack !\n");
        return;
    }

    // Initialize registers
    p_list[n_proc].pid              = n_proc;
    p_list[n_proc].regs.ss          = 0x33;
    p_list[n_proc].regs.esp         = 0x40001000;
    p_list[n_proc].regs.eflags      = 0x0;
    p_list[n_proc].regs.cs          = 0x23;
    p_list[n_proc].regs.eip         = 0x40000000;
    p_list[n_proc].regs.ds          = 0x2B;
    p_list[n_proc].regs.es          = 0x2B;
    p_list[n_proc].regs.fs          = 0x2B;
    p_list[n_proc].regs.gs          = 0x2B;
    p_list[n_proc].regs.cr3         = (uint32_t) pd;

    p_list[n_proc].user_stack.ss0   = 0x18;
    p_list[n_proc].user_stack.esp0  = user_stack_base + PAGE_SIZE;

    p_list[n_proc].regs.eax         = 0;
    p_list[n_proc].regs.ebx         = 0;
    p_list[n_proc].regs.ecx         = 0;
    p_list[n_proc].regs.edx         = 0;

    p_list[n_proc].regs.ebp         = 0;
    p_list[n_proc].regs.edi         = 0;
    p_list[n_proc].regs.esi         = 0;

    n_proc++;
}
