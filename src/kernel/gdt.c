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
    /*
     * Null descriptor
     */
    init_gdt_desc(0x00, 0x00,    0x00, 0x00, &kgdt[0]);

    /*
     * Code segment
     *
     *  base    -> 0x00:        Start at the beginning of the memory
     *  limit   -> 0x0FFFFF:    0xFFFFF + 1 so 0x100000 pages of 4 Ko -> 4 Go (all the memory)
     *  access bits:
     *      Accessed bit:               1
     *      Readable/Writable bit:      1
     *      Direction/Conforming bit:   0
     *      Executable bit:             1
     *
     *      bit 4:                      1
     *      Privilege:                  0
     *                                  0
     *      Present bit:                1
     *                                 --
     *                               0x9B
     * flag bits:
     *      bit 0:                      1
     *      bit 1:                      0
     *      Size bit:                   1
     *      Granularity bit:            1
     *                                 --
     *                                0xD
     */
    init_gdt_desc(0x00, 0xFFFFF, 0x9B, 0x0D, &kgdt[1]);

    /*
     * Data segment
     *
     *  base    -> 0x00:        Start at the beginning of the memory
     *  limit   -> 0x0FFFFF:    0xFFFFF + 1 so 0x100000 pages of 4 Ko -> 4 Go (all the memory)
     *  access bits:
     *      Accessed bit:               1
     *      Readable/Writable bit:      1
     *      Direction/Conforming bit:   0
     *      Executable bit:             0
     *
     *      bit 4:                      1
     *      Privilege:                  0
     *                                  0
     *      Present bit:                1
     *                                 --
     *                               0x93
     * flag bits:
     *      bit 0:                      1
     *      bit 1:                      0
     *      Size bit:                   1
     *      Granularity bit:            1
     *                                 --
     *                                0xD
     */
    init_gdt_desc(0x00, 0xFFFFF, 0x93, 0x0D, &kgdt[2]);

    /*
     * Stack segment
     *
     *  base    -> 0x00:        Not used by the GDT because it is the stack
     *  limit   -> 0x00:        Limit of the stack but because it is the stack, the limit is an inferior limit so the stack is accross all the memory
     *  access bits:
     *      Accessed bit:               1
     *      Readable/Writable bit:      1
     *      Direction/Conforming bit:   1
     *      Executable bit:             0
     *
     *      bit 4:                      1
     *      Privilege:                  0
     *                                  0
     *      Present bit:                1
     *                                 --
     *                               0x97
     * flag bits:
     *      bit 0:                      1
     *      bit 1:                      0
     *      Size bit:                   1
     *      Granularity bit:            1
     *                                 --
     *                                0xD
     */
    init_gdt_desc(0x00, 0x00,    0x97, 0x0D, &kgdt[3]);

    kgdtr.size   = GDT_SIZE * 8;
    kgdtr.offset = GDT_BASE;

    memcpy((char*) kgdtr.offset, (char*) kgdt, kgdtr.size);

    asm("lgdtl (kgdtr)");

    asm("movw $0x10, %ax    \n \
         movw %ax, %ds      \n \
         movw %ax, %es      \n \
         movw %ax, %fs      \n \
         movw %ax, %gs      \n \
         ljmp $0x08, $next  \n \
         next:              \n");
}
