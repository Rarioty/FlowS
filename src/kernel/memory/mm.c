#include <kernel/utils/registers.h>
#include <kernel/interrupts/isrs.h>
#include <kernel/process/panic.h>
#include <kernel/memory/malloc.h>
#include <stddef.h>
#include <stdio.h>

#define __MM__
#include <kernel/memory/mm.h>

void page_fault(irq_registers* r)
{
	uint32_t faulting_address;
	asm volatile("mov %%cr2, %0" : "=r"(faulting_address));

    int present  = !(r->err_code & 0x1) ? 1 : 0;
	int rw       = r->err_code & 0x2    ? 1 : 0;
	int user     = r->err_code & 0x4    ? 1 : 0;
	int reserved = r->err_code & 0x8    ? 1 : 0;
	int id       = r->err_code & 0x10   ? 1 : 0;

	printf("Segmentation fault. (p:%d,rw:%d,user:%d,res:%d,id:%d) at 0x%x eip:0x%x\n",
			present, rw, user, reserved, id, faulting_address, r->eip);
}

void general_protection_fault(irq_registers* r)
{
	// Decode the selector error code: http://wiki.osdev.org/Exceptions#Selector_Error_Code
	uint16_t index;
	uint16_t tbl;
	uint16_t e;

	index 	= (r->err_code & 0x0000FFF8) >> 3;
	tbl   	= (r->err_code & 0x00000003) >> 1;
	e		= (r->err_code & 0x00000001);
	printf("General protection fault (error_code: %04X) on EIP=%08X.\n", r->err_code, r->eip);
	printf("    External:   %d\n", e);
	if (tbl == 0)
		printf("    Descriptor: GDT\n");
	else if (tbl == 1)
		printf("    Descriptor: IDT\n");
	else if (tbl == 2)
		printf("    Descriptor: LDT\n");
	else if (tbl == 3)
		printf("    Descriptor: IDT\n");
	printf("    Index:      0x%X\n", index);

	PANIC("General protection fault dump", r);
}

char* get_page_frame(void)
{
    int byte, bit;
    int page = -1;

    for (byte = 0; byte < RAM_MAXPAGE / 8; ++byte)
        if (mem_bitmap[byte] != 0xFF)
            for (bit = 0; bit < 8; ++bit)
                if (!(mem_bitmap[byte] & (1 << bit)))
                {
                    page = 8 * byte + bit;
                    set_page_frame_used(page);
                    return (char*) (page * PAGE_SIZE);
                }

    return (char*)-1;
}

/**
 * Search a free page in the virtual memory of the kernel. Then the function
 * ask for a physical page. As one page is in the kernel space, this last
 * is updated.
 */
struct page* get_page_from_heap(void)
{
	char *v_addr, *p_addr;
	struct vm_area* p;
	struct page* pg;

	// Take a free physical page
	p_addr = get_page_frame();
	if ((int)p_addr < 0)
	{
		PANIC("No page frame available. System halted !", NULL);
		asm("hlt");
	}

	// Check if a virtual page is free
	if (free_vm->vm_end == free_vm->vm_start)
	{
		PANIC("Not memory left in page heap. System halted !", NULL);
		asm("hlt");
	}

	// Take a virtual free page
	v_addr = free_vm->vm_start;

	if (free_vm->vm_end - free_vm->vm_start == PAGE_SIZE)
	{
		if (free_vm->next)
		{
			p 				= free_vm;
			free_vm 		= free_vm->next;
			free_vm->prev 	= NULL;
			free(p);
		}
	}
	else
	{
		free_vm->vm_start += PAGE_SIZE;
	}

	// Update the kernel address space
	kernel_add_page(v_addr, p_addr, 0);

	// Return the page
	pg = (struct page*) malloc(sizeof(struct page));
	pg->v_addr = v_addr;
	pg->p_addr = p_addr;

	return pg;
}

int release_page_from_heap(char* v_addr)
{
	struct vm_area *p, *to_del, *new_vm_area;
	char* p_addr;

	// Get the associated frame of v_addr and free it
	p_addr = get_p_addr(v_addr);
	if (p_addr)
		release_page_frame(p_addr);
	else
	{
		PANIC("No page frame associated with v_addr. System halted !", NULL);
		asm("hlt");
	}

	// Update the page directory
	pd_remove_page(v_addr);

	// Update the list of free virtual addresses
	p = free_vm;
	while (p->vm_start < v_addr && p->next)
		p = p->next;

	if (v_addr + PAGE_SIZE == p->vm_start)
	{
		p->vm_start = v_addr;
		if (p->prev  && p->prev->vm_end == p->vm_start)
		{
			to_del = p->prev;
			p->vm_start = p->prev->vm_start;
			p->prev = p->prev->prev;
			if (p->prev)
				p->prev->next = p;
			free(to_del);
		}
	}
	else if (p->prev && p->prev->vm_end == v_addr)
	{
		p->prev->vm_end += PAGE_SIZE;
	}
	else if (v_addr + PAGE_SIZE < p->vm_start)
	{
		new_vm_area = (struct vm_area*) malloc(sizeof(struct vm_area));
		new_vm_area->vm_start = v_addr;
		new_vm_area->vm_end = v_addr + PAGE_SIZE;
		new_vm_area->prev = p->prev;
		new_vm_area->next = p;
		p->prev = new_vm_area;
		if (new_vm_area->prev)
			new_vm_area->prev->next = new_vm_area;
	}
	else
	{
		PANIC("Corrupted linked list. System halted !", NULL);
		asm("hlt");
	}

	return 1;
}

void init_mm(uint32_t memory_high)
{
    uint32_t pg, pg_limit;
	unsigned long i;

	// Get the number of the last page
	pg_limit = (memory_high * 1024) / PAGE_SIZE;

    // Initialize bitmap of physical pages
    for (pg = 0; pg < pg_limit / 8; ++pg)
	{
    	mem_bitmap[pg] = 0;
	}

	for (pg = pg_limit / 8; pg < RAM_MAXPAGE / 8; ++pg)
	{
		mem_bitmap[pg] = 0xFF;
	}

    // Reserve pages for the core
    for (pg = PAGE(0x0); pg < PAGE((uint32_t)pg1_end); ++pg)
        set_page_frame_used(pg);

    // Initialize page directory
	pd0[0] = ((uint32_t) pg0 | (PG_PRESENT | PG_WRITE | PG_4MB));
	pd0[1] = ((uint32_t) pg1 | (PG_PRESENT | PG_WRITE | PG_4MB));
    for (i = 2; i < 1023; ++i)
		pd0[i] = ((uint32_t)pg1 + PAGE_SIZE * i) | (PG_PRESENT | PG_WRITE);

	// Page table mirroring magic trick !
	pd0[1023] = ((uint32_t) pd0 | (PG_PRESENT | PG_WRITE));

	// Switch pages state on
    asm("mov %0, %%eax      \n  \
         mov %%eax, %%cr3   \n  \
		 mov %%cr4, %%eax	\n	\
		 or %2, %%eax		\n	\
		 mov %%eax, %%cr4	\n	\
         mov %%cr0, %%eax   \n  \
         or %1, %%eax       \n  \
         mov %%eax, %%cr0   \n"
         :: "m" (pd0), "i" (PAGING_FLAG), "i" (PSE_FLAG));

	// Initalize kernel heap used by malloc
	kern_heap = (char*) KERNEL_HEAP;
	sbrk(1);

	// Initialize the list of virtual memory addresses
	free_vm = (struct vm_area*) malloc(sizeof(struct vm_area));
	free_vm->vm_start = (char*) KERNEL_HEAP_PAGE;
	free_vm->vm_end = (char*) KERNEL_HEAP_PAGE_LIMIT;
	free_vm->next = NULL;
	free_vm->prev = NULL;

	isrs_install_handler(13, general_protection_fault);
    isrs_install_handler(14, page_fault);
}

struct page_directory* pd_create(void)
{
    struct page_directory* pd;
	uint32_t* pdir;
	int i;

	// Take and initialize a page for page_directory
	pd = (struct page_directory*) malloc(sizeof(struct page_directory));
	pd->base = get_page_from_heap();

	/*
	 * Kernel space.
	 * The virtual addresses under USER_OFFSET are referenced to the kernel pages !
	 */
	pdir = (uint32_t*) pd->base->v_addr;
	for (i = 0; i < 256; ++i)
	{
		pdir[i] = pd0[i];
	}

	/*
	 * User space.
	 */
	for (i = 256; i < 1023; ++i)
	{
		pdir[i] = 0;
	}

	// Page table mirroring magic trick !
	pdir[1023] = ((uint32_t) pd->base->p_addr | (PG_PRESENT | PG_WRITE));

	// Update the tables pages list of the user space
	pd->pt = 0;

	return pd;
}

int pd_destroy(struct page_directory* pd)
{
	struct page_list *pgh, *oldpgh;

	// Free the corresponding page of the directory
	release_page_from_heap(pd->base->v_addr);

	// Free the pages of the tables
	pgh = pd->pt;

	while (pgh)
	{
		release_page_from_heap(pgh->page->v_addr);
		oldpgh = pgh;
		pgh = pgh->next;
		free(oldpgh);
	}

	free(pd);

	return 1;
}

int kernel_add_page(char* v_addr, char* p_addr, int flags)
{
	uint32_t* pde;
	uint32_t* pte;

	if (v_addr > (char*) USER_OFFSET)
	{
		printf("ERROR: %p is not in kernel space !\n", v_addr);
		return 0;
	}

	// Verify that the page exists
	pde = (uint32_t*) (0xFFFFF000 | (((uint32_t) v_addr & 0xFFC00000) >> 20));
	if ((*pde & PG_PRESENT) == 0)
	{
		PANIC("Kernel page table not found for v_addr. System halted !", NULL);
		asm("hlt");
	}

	// Modify the entry in page table
	pte  = (uint32_t*) (0xFFC00000 | (((uint32_t) v_addr & 0xFFFFF000) >> 10));
	*pte = ((uint32_t) p_addr) | (PG_PRESENT | PG_WRITE | flags);

	return 0;
}

int pd_add_page(char* v_addr, char* p_addr, int flags, struct page_directory* pd)
{
	uint32_t* pde;
	uint32_t* pte;
	uint32_t* pt;
	struct page* newpg;
	struct page_list* pglist;
	int i;

	pde = (uint32_t *) (0xFFFFF000 | (((uint32_t) v_addr & 0xFFC00000) >> 20));

	if ((*pde & PG_PRESENT) == 0)
	{
		newpg = get_page_from_heap();

		pt = (uint32_t*) newpg->v_addr;
		for (i = 1; i < 1024; ++i)
			pt[i] = 0;

		*pde = (uint32_t) newpg->p_addr | (PG_PRESENT | PG_WRITE | flags);

		if (pd)
		{
			if (pd->pt)
			{
				pglist = (struct page_list*) malloc(sizeof(struct page_list));
				pglist->page = newpg;
				pglist->next = pd->pt;
				pglist->prev = 0;
				pd->pt->prev = pglist;
				pd->pt = pglist;
			}
			else
			{
				pd->pt = (struct page_list*) malloc(sizeof(struct page_list));
				pd->pt->page = newpg;
				pd->pt->next = 0;
				pd->pt->prev = 0;
			}
		}
	}

	pte  = (uint32_t*) (0xFFC00000 | (((uint32_t) v_addr & 0xFFFFF000) >> 10));
	*pte = ((uint32_t) p_addr) | (PG_PRESENT | PG_WRITE | flags);

	return 0;
}

int pd_remove_page(char* v_addr)
{
	uint32_t* pte;

	if (get_p_addr(v_addr))
	{
		pte = (uint32_t*) (0xFFC00000 | (((uint32_t) v_addr & 0xFFFFF000) >> 10));
		*pte = (*pte & (~PG_PRESENT));
		asm("invlpg %0" :: "m"(v_addr));
	}

	return 0;
}

char* get_p_addr(char* v_addr)
{
	uint32_t* pde;
	uint32_t* pte;

	pde = (uint32_t*) (0xFFFFF000 | (((uint32_t) v_addr & 0xFFC00000) >> 20));
	if ((*pde & PG_PRESENT))
	{
		pte = (uint32_t*) (0xFFC00000 | (((uint32_t) v_addr & 0xFFFFF000) >> 10));
		if ((*pte & PG_PRESENT))
			return (char*) ((*pte & 0xFFFFF000) + (VADDR_PG_OFFSET((uint32_t) v_addr)));
	}

	return 0;
}
