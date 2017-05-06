#ifndef _IDT_H_
#define _IDT_H_

#include <stddef.h>

#define IDT_BASE    0x800
#define IDT_SIZE    0xFF

#define INTGATE     0x8E00

struct idtdesc
{
    uint16_t offset0_15;
    uint16_t select;
    uint16_t type;
    uint16_t offset16_31;
} __attribute__((packed));

struct idtr
{
    uint16_t limite;
    uint32_t base;
} __attribute__((packed));

struct idtr kidtr;
struct idtdesc kidt[IDT_SIZE];

void init_idt_desc(uint16_t, uint32_t, uint16_t, struct idtdesc*);
void init_idt(void);

#endif
