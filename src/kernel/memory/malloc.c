#include <kernel/memory/malloc.h>

#include <kernel/memory/memory_map.h>
#include <kernel/process/panic.h>
#include <kernel/memory/mm.h>

void* sbrk(int n)
{
    struct malloc_header* chunk;
    char* p_addr;
    int i;

    if ((kern_heap + (n * PAGE_SIZE)) > (char*) KERNEL_HEAP_LIMIT)
    {
        PANIC("No virtual memory left for kernel heap !", NULL);
        return (char*) -1;
    }

    chunk = (struct malloc_header*) kern_heap;

    // Allocate a free page
    for (i = 0; i < n; ++i)
    {
        p_addr = get_page_frame();
        if ((int)p_addr < 0)
        {
            PANIC("No free page available !", NULL);
            return (char*) -1;
        }

        // Add it to the page directory
        kernel_add_page(kern_heap, p_addr, 0);

        kern_heap += PAGE_SIZE;
    }

    // Flag it for malloc
    chunk->size = PAGE_SIZE * n;
    chunk->used = 0;

    return chunk;
}

void* malloc(unsigned long size)
{
    unsigned long realsize; // Total size of the record
    struct malloc_header *chunk, *other;

    if ((realsize = sizeof(struct malloc_header) + size) < MALLOC_MINSIZE)
        realsize = MALLOC_MINSIZE;

    // Search for the block
    chunk = (struct malloc_header*) KERNEL_HEAP;
    while (chunk->used || chunk->size < realsize)
    {
        if (chunk->size == 0)
        {
            PANIC("Corrupted chunk with null size ! System halted !", NULL);
            asm("hlt");
        }

        chunk = (struct malloc_header*) ((char*) chunk + chunk->size);

        if (chunk == (struct malloc_header*) kern_heap)
        {
            if ((int)(sbrk((realsize / PAGE_SIZE) + 1)) < 0)
            {
                PANIC("No memory left for kernel ! System halted !\n", NULL);
                asm("hlt");
            }
        }
        else if (chunk > (struct malloc_header*) kern_heap)
        {
            PANIC("Chunk above heap limit ! System halted !\n", NULL);
            asm("hlt");
        }
    }

    if (chunk->size - realsize < MALLOC_MINSIZE)
    {
        chunk->used = 1;
    }
    else
    {
        other = (struct malloc_header*) ((char*) chunk + realsize);
        other->size = chunk->size - realsize;
        other->used = 0;

        chunk->size = realsize;
        chunk->used = 1;
    }

    // Return a pointer of this space
    return (char*) chunk + sizeof(struct malloc_header);
}

void free(void* v_addr)
{
    struct malloc_header *chunk, *other;

    // Free the allocated block
    chunk = (struct malloc_header*) (v_addr - sizeof(struct malloc_header));
    chunk->used = 0;

    // Merge the free blocks
    while ((other = (struct malloc_header*) ((char*) chunk + chunk->size))
         && other < (struct malloc_header*) kern_heap
         && other ->used == 0)
    {
        chunk->size += other->size;
    }
}
