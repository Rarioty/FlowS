#ifndef _MM_H_
#define _MM_H_

#include <kernel/memory/memory_map.h>
#include <stddef.h>

/**
 * \file    mm.h
 * \brief   This file handle the whole paging system
 */

#define PAGE_SIZE               4096            /*!<    Size of page in the OS          */
#define	RAM_MAXSIZE	            0x100000000     /*!<    Maximum size of the RAM         */
#define RAM_MAXPAGE             0x100000        /*!<    Max pages in memory             */

#define VADDR_PD_OFFSET(addr)   ((addr) & 0xFFC00000) >> 22     /*!<    Macro to retrieve page directory offset from a linear address   */
#define VADDR_PT_OFFSET(addr)   ((addr) & 0x003FF000) >> 12     /*!<    Macro to retrieve page table offset from a linear address       */
#define VADDR_PG_OFFSET(addr)   ((addr) & 0x00000FFF)           /*!<    Macro to retrieve page offset from a linear address             */
#define PAGE(addr)              (addr) >> 12                    /*!<    Macro to retrieve page number from an address                   */

#define PG_PRESENT	            0x00000001	/* page directory / table */
#define PG_WRITE	            0x00000002
#define PG_USER		            0x00000004
#define PG_4MB		            0x00000080

#define PAGING_FLAG             0x80000000      /*!<    Bit 31 on for CR0                   */
#define PSE_FLAG	            0x00000010	    /*!<    Bit 4 on for CR4                    */
#define USER_OFFSET             0x40000000      /*!<    User code virtual memory location   */
#define USER_STACK              0xE0000000      /*!<    User stack virtual memory location  */

#ifndef __MM_STRUCT__
#define __MM_STRUCT__

/**
 * \struct  page
 * \brief   This structure represents a single page
 */
struct page
{
    char*               v_addr;         /*!<    Virtual address                             */
    char*               p_addr;         /*!<    Physical address                            */
};

/**
 * \struct  page_list
 * \brief   This structure handle a list of pages
 */
struct page_list
{
    struct page*        page;           /*!<    Start of the page list                      */
    struct page_list*   next;           /*!<    Link to the next page list                  */
    struct page_list*   prev;           /*!<    Link to the previous page list              */
};

/**
 * \struct  page_directory
 * \brief   This structure represents a page directory (obvious ?)
 */
struct page_directory
{
    struct page*        base;           /*!<    First page of the page directory            */
    struct page_list*   pt;             /*!<    Link to the first page list                 */
};

/**
 * \struct  vm_area
 * \brief   This structure handle a virtual memory area
 */
struct vm_area
{
    char*               vm_start;       /*!<    Link to the start of area                   */
    char*               vm_end;         /*!<    Link to the end of the area                 */
    struct vm_area*     next;           /*!<    Next virtual memory area                    */
    struct vm_area*     prev;           /*!<    Previous virtual memory area                */
};

#endif

char*               kern_heap;          /*!<    Top of the kernel heap                              */
struct vm_area*     free_vm;            /*!<    Pointer to the start of free virtual memory areas   */

#ifdef __MM__
    uint32_t*           pd0     = (uint32_t*) KERNEL_PAGE_DIRECTORY_ADDRESS;    /*!<    Kernel page directory               */
    char*               pg0     = (char*) 0;                                    /*!<    Kernel first page                   */
    char*               pg1     = (char*) 0x400000;                             /*!<    Kernel second page (4Mo)            */
    char*               pg1_end = (char*) 0x800000;                             /*!<    Kernel end of second page           */
    uint8_t             mem_bitmap[RAM_MAXPAGE / 8];                            /*!<    Bitmap for current page usage       */
#else
    extern uint32_t*    pd0;                                                    /*!<    Kernel page directory               */
    extern uint8_t      mem_bitmap[];                                           /*!<    Bitmap for current page usage       */
#endif

#define set_page_frame_used(page)   \
    mem_bitmap[((uint32_t)page)/8] |= (1 << (((uint32_t)page) % 8))                                 /*!<    Macro to set the used bit of a page     */
#define release_page_frame(p_addr)  \
    mem_bitmap[((uint32_t)p_addr/PAGE_SIZE)/8] &= ~(1 << (((uint32_t) p_addr / PAGE_SIZE) % 8))     /*!<    Macro to unset the used bit of a page   */

/**
 * \brief   Get a unused page an return a pointer to it
 *
 * \return  Pointer to an unused page
 */
char* get_page_frame(void);

/**
 * \brief   Select a free page in the bitmap and associate it to a virtual page in the heap
 *
 * \return  The selected page
 */
struct page* get_page_from_heap(void);

/**
 * \brief   Free a page in the bitmap
 *
 * \param[in] v_addr    Virtual address of the page
 *
 * \return  error code
 */
int release_page_from_heap(char* v_addr);

/**
 * \brief   Initialize the paging system
 *
 * \param[in] memory_high   Upper bound of memory given by GRUB
 */
void init_mm(uint32_t memory_high);

/**
 * \brief   Create a page directory for a process
 *
 * \return  The created page directory
 */
struct page_directory* pd_create(void);

/**
 * \brief   Destroy a page directory after usage
 *
 * \param[in] page_directory    The page directory to destroy
 *
 * \return  error code
 */
int pd_destroy(struct page_directory* page_directory);

/**
 * \brief   Add a page to the kernel page directory
 *
 * \param[in] v_addr    Virtual address for the page
 * \param[in] p_addr    Physical address for the page
 * \param[in] flags     Flags for the page
 *
 * \return  Error code
 */
int kernel_add_page(char* v_addr, char* p_addr, int flags);

/**
 * \brief   Add a page to a specified page directory
 *
 * \param[in] v_addr            Virtual address for the page
 * \param[in] p_addr            Physical address for the page
 * \param[in] flags             Flags for the page
 * \param[in] page_directory    The page directory we want to put the page in
 *
 * \return  Error code
 */
int pd_add_page(char* v_addr, char* p_addr, int flags, struct page_directory* page_directory);

/**
 * \brief   Remove a page from a page directory
 *
 * \param[in] v_addr    Virtual address of the page
 *
 * \return  Error code
 */
int pd_remove_page(char* v_addr);

/**
 * \brief   Retrieve the physical address of a virtual address
 *
 * \param[in] v_addr    The virtual address we want to convert
 *
 * \return  The resulting physical address
 */
char* get_p_addr(char* v_addr);

#endif
