#include <kernel/mm.h>
#include <string.h>

#define __PLIST__
#include <kernel/process.h>

void load_task(uint32_t* code_phys_addr, uint32_t* fn, unsigned int code_size)
{
    uint32_t page_base, pages, i;
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

    // Initialize registers
    p_list[n_proc].pid      = n_proc;
    p_list[n_proc].regs.ss  = 0x33;
    p_list[n_proc].regs.esp = 0x40001000;
    p_list[n_proc].regs.cs  = 0x23;
    p_list[n_proc].regs.eip = 0x40000000;
    p_list[n_proc].regs.ds  = 0x2B;
    p_list[n_proc].regs.cr3 = (uint32_t) pd;

    n_proc++;
}
