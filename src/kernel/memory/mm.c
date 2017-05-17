#include <kernel/utils/registers.h>
#include <kernel/interrupts/isrs.h>
#include <stddef.h>
#include <stdio.h>

#define __MM__
#include <kernel/memory/mm.h>

uint32_t*   page_directory;
uint32_t*   first_page_table;

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
	while(1);
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

void init_mm(void)
{
    uint32_t page_addr;
    int i, pg;

    // Initialize bitmap
    for (pg = 0; pg < RAM_MAXPAGE / 8; ++pg)
        mem_bitmap[pg] = 0;

    // Reserve pages for the core
    for (pg = PAGE(0x0); pg < PAGE(0x20000); ++pg)
        set_page_frame_used(pg);

    // Reserver pages for the hardware
    for (pg = PAGE(0xA0000); pg < PAGE(0x100000); ++pg)
        set_page_frame_used(pg);

    // Take a page for the page directory and the first page table
    page_directory   = (uint32_t*)get_page_frame();
    first_page_table = (uint32_t*)get_page_frame();

    // Initialize page directory
    page_directory[0]  = (uint32_t)first_page_table;
    page_directory[0] |= 3;
    for (i = 1; i < 1024; ++i)
        page_directory[i] = 0;

    // Initialize the first page table
    page_addr = 0;
    for (pg = 0; pg < 1024; ++pg)
    {
        first_page_table[pg]  = page_addr;
        first_page_table[pg] |= 3;
        page_addr += 4096;
    }

    asm("mov %0, %%eax      \n  \
         mov %%eax, %%cr3   \n  \
         mov %%cr0, %%eax   \n  \
         or %1, %%eax       \n  \
         mov %%eax, %%cr0   \n"
         :: "m" (page_directory), "i" (PAGING_FLAG));

	isrs_install_handler(13, general_protection_fault);
    isrs_install_handler(14, page_fault);
}

uint32_t* pd_create(uint32_t* code_phys_addr, unsigned int code_size)
{
    uint32_t *pd, *pt;
    uint32_t i, j;
    uint32_t pages;

    // Take a page and initialize the page directory
    pd = (uint32_t*)get_page_frame();
    for (i = 1; i < 1024; ++i)
        pd[i] = 0;

    // Kernel space
    pd[0]  = page_directory[0];
    pd[0] |= 3;

    // User space
    if (code_size % PAGE_SIZE)
        pages = code_size / PAGE_SIZE + 1;
    else
        pages = code_size / PAGE_SIZE;

    for (i = 0; i < pages; ++i)
    {
        pt = (uint32_t*) get_page_frame();

        pd[(USER_OFFSET + i * PAGE_SIZE * 1024) >> 22]  = (uint32_t)pt;
        pd[(USER_OFFSET + i * PAGE_SIZE * 1024) >> 22] |= 7;

        for (j = 0; j < 1024 && pages; ++j, --pages)
        {
            pt[j]  = (uint32_t) (code_phys_addr + i * PAGE_SIZE * 1024 + j * PAGE_SIZE);
            pt[j] |= 7;
        }
    }

    return pd;
}
