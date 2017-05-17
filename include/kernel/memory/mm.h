#ifndef _MM_H_
#define _MM_H_

#include <stddef.h>

/**
 * \file    mm.h
 * \brief   This file handle the whole paging system
 */

#define PAGE_SIZE               4096            /*!<    Size of page in the OS          */
#define RAM_MAXPAGE             0x10000         /*!<    Max pages in memory             */

#define VADDR_PD_OFFSET(addr)   ((addr) & 0xFFC00000) >> 22     /*!<    Macro to retrieve page directory offset from a linear address   */
#define VADDR_PT_OFFSET(addr)   ((addr) & 0x003FF000) >> 12     /*!<    Macro to retrieve page table offset from a linear address       */
#define VADDR_PG_OFFSET(addr)   ((addr) & 0x00000FFF)           /*!<    Macro to retrieve page offset from a linear address             */
#define PAGE(addr)              (addr) >> 12                    /*!<    Macro to retrieve page number from an address                   */

#define PAGING_FLAG             0x80000000      /*!<    Bit 31 on for CR0                   */
#define USER_OFFSET             0x40000000      /*!<    User code virtual memory location   */
#define USER_STACK              0xE0000000      /*!<    User stack virtual memory location  */

#ifdef __MM__
    uint8_t mem_bitmap[RAM_MAXPAGE / 8];        /*!<    Bitmap for current page usage       */
#else
    extern uint8_t mem_bitmap[];                /*!<    Bitmap for current page usage       */
#endif

#define set_page_frame_used(page)   \
    mem_bitmap[((uint32_t)page)/8] |= (1 << (((uint32_t)page) % 8))                                 /*!<    Macro to set the used bit of a page     */
#define release_page_frame(p_addr)  \
    mem_bitmap[((uint32_t)p_addr/PAGE_SIZE)/8] &= ~(1 << (((uint32_t) p_addr / PAGE_SIZE) % 8))     /*!<    Macro to unset the used bit of a page   */

/**
 * \struct  pd_entry
 * \brief   This structure represent a page directory entry
 */
struct pd_entry
{
    uint32_t present:   1;
    uint32_t writable:  1;
    uint32_t user:      1;
    uint32_t pwt:       1;
    uint32_t pcd:       1;
    uint32_t accessed:  1;
    uint32_t _unused:   1;
    uint32_t page_size: 1;
    uint32_t global:    1;
    uint32_t avail:     4;

    uint32_t page_table_base: 20;
} __attribute__((packed));

/**
 * \struct  pt_entry
 * \brief   This structure represent a page table entry
 */
struct pt_entry
{
    uint32_t present:   1;
    uint32_t writable:  1;
    uint32_t user:      1;
    uint32_t pwt:       1;
    uint32_t pcd:       1;
    uint32_t accessed:  1;
    uint32_t dirty:     1;
    uint32_t pat:       1;
    uint32_t global:    1;
    uint32_t avail:     4;

    uint32_t page_base: 20;
} __attribute__((packed));

/**
 * \brief   Get a unused page an return a pointer to it
 *
 * \return  Pointer to an unused page
 */
char* get_page_frame(void);

/**
 * \brief   Initialize the paging system
 */
void init_mm(void);

/**
 * \brief   Create a page directory entry with all associated page tables
 *
 * \param[in] code_phys_addr    Physical address of the code to set in the page table
 * \param[in] code_size         Size of the code to know how much pages we need
 *
 * \return  Pointer to the new page directory
 */
uint32_t* pd_create(uint32_t* code_phys_addr, unsigned int code_size);

#endif
