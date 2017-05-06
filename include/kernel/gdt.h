#ifndef _GDT_H_
#define _GDT_H_

#include <stddef.h>

#define GDT_BASE    0x00
#define GDT_SIZE    0xFF

struct gdtdesc
{
    uint16_t lim0_15;
    uint16_t base0_15;
    uint8_t base16_23;
    uint8_t access;
    uint8_t lim16_19:4;
    uint8_t other:4;
    uint8_t base24_31;
} __attribute__((packed));

struct gdtr
{
    uint16_t limite;
    uint32_t base;
} __attribute__((packed));

void init_gdt_desc(uint32_t base, uint32_t limite, uint8_t access, uint8_t other, struct gdtdesc* desc);
void init_gdt(void);

#ifdef __KGDT_C_FILE__
    struct gdtdesc kgdt[GDT_SIZE];
    struct gdtr kgdtr;
#else
    extern struct gdtdesc kgdt[];
    extern struct gdtr kgdtr;
#endif

#endif
