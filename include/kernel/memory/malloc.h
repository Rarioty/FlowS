#ifndef _MALLOC_H_
#define _MALLOC_H_

#include <stddef.h>

#define     MALLOC_MINSIZE      16

struct malloc_header
{
    uint32_t    size:31;
    uint32_t    used:1;
} __attribute__((packed));

/**
 * \brief   Extend memory area used by process
 *
 * \param[in] n     Number of pages we want to add
 *
 * \return  Pointer to the new end of the memory area
 */
void* sbrk(int n);

/**
 * \brief   Allocate memory in the heap for the process
 *
 * \param[in] size  Size of the memory area we want
 *
 * \return  Pointer to this new memory area
 */
void* malloc(unsigned long size);

/**
 * \brief   Free allocated memory
 *
 * \param[in] v_addr    Pointer to the allocated memory area
 */
void  free(void* v_addr);

#endif
