#include <kernel/process/panic.h>
#include <kernel/memory/malloc.h>
#include <kernel/memory/mm.h>
#include <string.h>
#include <stdio.h>

#define __PLIST__
#include <kernel/process/process.h>

int load_task(char* fn, uint32_t code_size)
{
    struct page_directory* pd;
    struct page_list* pglist;
    struct page* kernel_stack;

    char* v_addr;
    char* p_addr;
    char* user_stack;

    uint32_t i;
    int pid;

    // Compute new pid
    pid = 1;
	while (p_list[pid].state != 0 && pid++ < MAX_PID);

    if (p_list[pid].state != 0)
    {
        PANIC("Not enough slot for processes !\n", NULL);
        return 0;
    }

    // Create the page directory
    pd = pd_create();

    //Go to the new address space
	asm("mov %0, %%eax; mov %%eax, %%cr3"::"m"(pd->base->p_addr));

    // Allocate pages to copy the task
    pglist = (struct page_list*) malloc(sizeof(struct page_list));
    pglist->page = 0;
    pglist->next = 0;
    pglist->prev = 0;

    i = 0;
    while (i < code_size)
    {
        // Create the address space for user above 0x40000000
        p_addr = get_page_frame();
        v_addr = (char*) (USER_OFFSET + i);
        pd_add_page(v_addr, p_addr, PG_USER, pd);

        // Update the page list
        pglist->page = (struct page*) malloc(sizeof(struct page));
        pglist->page->p_addr = p_addr;
        pglist->page->v_addr = v_addr;

        pglist->next = (struct page_list*) malloc(sizeof(struct page_list));
        pglist->next->page = 0;
        pglist->next->next = 0;
        pglist->next->prev = pglist;

        pglist = pglist->next;

        i += PAGE_SIZE;
    }

    // Copy the code to the specified memory location
    memcpy((char*) USER_OFFSET, fn, code_size);

    // Create the user stack
    user_stack = get_page_frame();
    pd_add_page((char*) USER_STACK, user_stack, PG_USER, pd);

    // Create the kernel stack
    kernel_stack = get_page_from_heap();

    n_proc++;

    p_list[pid].pid = pid;

    // Initialize registers
    p_list[pid].regs.ss             = 0x33;
    p_list[pid].regs.esp            = USER_STACK + PAGE_SIZE - 16;
    p_list[pid].regs.eflags         = 0x0;
    p_list[pid].regs.cs             = 0x23;
    p_list[pid].regs.eip            = 0x40000000;
    p_list[pid].regs.ds             = 0x2B;
    p_list[pid].regs.es             = 0x2B;
    p_list[pid].regs.fs             = 0x2B;
    p_list[pid].regs.gs             = 0x2B;

    p_list[pid].regs.cr3            = (uint32_t) pd->base->p_addr;

    p_list[pid].kernel_stack.ss0    = 0x18;
    p_list[pid].kernel_stack.esp0   = (uint32_t) kernel_stack->v_addr + PAGE_SIZE - 16;

    p_list[pid].regs.eax            = 0;
    p_list[pid].regs.ebx            = 0;
    p_list[pid].regs.ecx            = 0;
    p_list[pid].regs.edx            = 0;

    p_list[pid].regs.ebp            = 0;
    p_list[pid].regs.edi            = 0;
    p_list[pid].regs.esi            = 0;

    p_list[pid].pd                  = pd;
    p_list[pid].pglist              = pglist;

    p_list[pid].state               = 1;

    asm("mov %0, %%eax; mov %%eax, %%cr3" :: "m"(current->regs.cr3));

    return pid;
}
