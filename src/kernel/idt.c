#include <kernel/idt.h>

#include <string.h>

void _asm_default_int(void);
void _asm_exc_GP(void);
void _asm_irq_0(void);
void _asm_irq_1(void);

void init_idt_desc(uint16_t select, uint32_t offset, uint16_t type, struct idtdesc* desc)
{
    desc->offset0_15    = (offset & 0xffff);
    desc->select        = select;
    desc->type          = type;
    desc->offset16_31   = (offset & 0xffff0000) >> 16;
    return;
}

void init_idt(void)
{
    int i;

    for (i = 0; i < IDT_SIZE; ++i)
    {
        init_idt_desc(0x08, (uint32_t) _asm_default_int, INTGATE, &kidt[i]);
    }

    init_idt_desc(0x08, (uint32_t) _asm_exc_GP, INTGATE, &kidt[13]);

    init_idt_desc(0x08, (uint32_t) _asm_irq_0, INTGATE, &kidt[32]);
    init_idt_desc(0x08, (uint32_t) _asm_irq_1, INTGATE, &kidt[33]);

    kidtr.limite = IDT_SIZE * 8;
    kidtr.base   = IDT_BASE;

    memcpy ((char*) kidtr.base, (char*) kidt, kidtr.limite);

    asm("lidtl (kidtr)");
}
