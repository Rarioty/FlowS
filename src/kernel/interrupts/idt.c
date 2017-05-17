#include <kernel/interrupts/idt.h>

#include <kernel/interrupts/isrs.h>
#include <kernel/interrupts/irq.h>
#include <kernel/tty.h>
#include <string.h>
#include <stddef.h>

void _asm_default_int(void);
void isr_default_int(void)
{
    terminal_writeline("interrupt");
}

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
    irq_install();
    isrs_install();

    kidtr.limite = IDT_SIZE * 8;
    kidtr.base   = IDT_BASE;

    memcpy ((char*) kidtr.base, (char*) kidt, kidtr.limite);

    asm("lidtl (kidtr)");
}
