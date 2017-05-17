#include <kernel/interrupts/irq.h>

#include <kernel/utils/registers.h>
#include <kernel/interrupts/idt.h>
#include <kernel/memory/io.h>

extern void _asm_irq_0(void);
extern void _asm_irq_1(void);
extern void _asm_irq_2(void);
extern void _asm_irq_3(void);
extern void _asm_irq_4(void);
extern void _asm_irq_5(void);
extern void _asm_irq_6(void);
extern void _asm_irq_7(void);
extern void _asm_irq_8(void);
extern void _asm_irq_9(void);
extern void _asm_irq_10(void);
extern void _asm_irq_11(void);
extern void _asm_irq_12(void);
extern void _asm_irq_13(void);
extern void _asm_irq_14(void);
extern void _asm_irq_15(void);

static irq_handler_t irq_routines[16] = {NULL};

void irq_install_handler(int irq, irq_handler_t handler)
{
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq)
{
    irq_routines[irq] = NULL;
}

void irq_handler(irq_registers* r)
{
    void (*handler)(irq_registers *r);
    if (r->int_no > 47 || r->int_no < 32)
        handler = NULL;
    else
        handler = irq_routines[r->int_no - 32];

    if (handler)
        handler(r);
}

void irq_install(void)
{
    init_idt_desc(0x08, (uint32_t) _asm_irq_0, INTGATE, &kidt[32]);
    init_idt_desc(0x08, (uint32_t) _asm_irq_1, INTGATE, &kidt[33]);
    init_idt_desc(0x08, (uint32_t) _asm_irq_2, INTGATE, &kidt[34]);
    init_idt_desc(0x08, (uint32_t) _asm_irq_3, INTGATE, &kidt[35]);
    init_idt_desc(0x08, (uint32_t) _asm_irq_4, INTGATE, &kidt[36]);
    init_idt_desc(0x08, (uint32_t) _asm_irq_5, INTGATE, &kidt[37]);
    init_idt_desc(0x08, (uint32_t) _asm_irq_6, INTGATE, &kidt[38]);
    init_idt_desc(0x08, (uint32_t) _asm_irq_7, INTGATE, &kidt[39]);
    init_idt_desc(0x08, (uint32_t) _asm_irq_8, INTGATE, &kidt[40]);
    init_idt_desc(0x08, (uint32_t) _asm_irq_9, INTGATE, &kidt[41]);
    init_idt_desc(0x08, (uint32_t) _asm_irq_10, INTGATE, &kidt[42]);
    init_idt_desc(0x08, (uint32_t) _asm_irq_11, INTGATE, &kidt[43]);
    init_idt_desc(0x08, (uint32_t) _asm_irq_12, INTGATE, &kidt[44]);
    init_idt_desc(0x08, (uint32_t) _asm_irq_13, INTGATE, &kidt[45]);
    init_idt_desc(0x08, (uint32_t) _asm_irq_14, INTGATE, &kidt[46]);
    init_idt_desc(0x08, (uint32_t) _asm_irq_15, INTGATE, &kidt[47]);
}
