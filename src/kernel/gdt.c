#define __KGDT_C_FILE__
#include <kernel/gdt.h>

#include <string.h>

void init_gdt_desc(uint32_t base, uint32_t limite, uint8_t access, uint8_t other, struct gdtdesc* desc)
{
    desc->lim0_15   = (limite & 0xffff);
    desc->base0_15  = (base & 0xffff);
    desc->base16_23 = (base & 0xff0000) >> 16;
    desc->access    = access;
    desc->lim16_19  = (limite & 0xf0000) >> 16;
    desc->other     = (other & 0xf);
    desc->base24_31 = (base & 0xff000000) >> 24;

    return;
}

void init_gdt(void)
{
    init_gdt_desc(0x00, 0x00,    0x00, 0x00, &kgdt[0]);
    init_gdt_desc(0x00, 0xFFFFF, 0x9B, 0x0D, &kgdt[1]);
    init_gdt_desc(0x00, 0xFFFFF, 0x93, 0x0D, &kgdt[2]);
    init_gdt_desc(0x00, 0x00,    0x97, 0x0D, &kgdt[3]);

    kgdtr.limite = GDT_SIZE * 8;
    kgdtr.base   = GDT_BASE;

    memcpy((char*) kgdtr.base, (char*) kgdt, kgdtr.limite);

    asm("lgdtl (kgdtr)");

    asm("movw $0x10, %ax    \n \
         movw %ax, %ds      \n \
         movw %ax, %es      \n \
         movw %ax, %fs      \n \
         movw %ax, %gs      \n \
         ljmp $0x08, $next  \n \
         next:              \n");
}
