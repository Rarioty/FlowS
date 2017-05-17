#define __KGDT_C_FILE__
#include <kernel/memory/gdt.h>

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
     * Initialize the default tss
     */
    default_tss.debug_flag      = 0x00;
    default_tss.io_map          = 0x00;
    default_tss.esp0            = 0x20000;
    default_tss.ss0             = 0x18;

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
     *  base    -> 0x020000:    Start of the stack
     *  limit   -> 0x10:        0x10 pages of 4KiB -> 64 KiB of stack
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
    init_gdt_desc(0x0, 0x00, 0x97, 0x0D, &kgdt[3]);

    /*
     * Users segment descriptors
     */

     /*
      * User code segment
      *
      *  base    -> 0x030000:    Start at 0x030000 in memory
      *  limit   -> 0x01:        1 page of 4 KiB is allowed for code
      *  access bits:
      *      Accessed bit:               1
      *      Readable/Writable bit:      1
      *      Direction/Conforming bit:   1
      *      Executable bit:             1
      *
      *      bit 4:                      1
      *      Privilege:                  1
      *                                  1
      *      Present bit:                1
      *                                 --
      *                               0xFF
      * flag bits:
      *      bit 0:                      1
      *      bit 1:                      0
      *      Size bit:                   1
      *      Granularity bit:            1
      *                                 --
      *                                0xD
      */
     init_gdt_desc(0x0, 0xFFFFF, 0xFF, 0x0D, &kgdt[4]);

    /*
     * User data segment
     *
     *  base    -> 0x030000:    Start at 0x030000 in memory
     *  limit   -> 0x01:        1 page of 4 KiB is allowed for code
     *  access bits:
     *      Accessed bit:               1
     *      Readable/Writable bit:      1
     *      Direction/Conforming bit:   0
     *      Executable bit:             0
     *
     *      bit 4:                      1
     *      Privilege:                  1
     *                                  1
     *      Present bit:                1
     *                                 --
     *                               0xF3
     * flag bits:
     *      bit 0:                      1
     *      bit 1:                      0
     *      Size bit:                   1
     *      Granularity bit:            1
     *                                 --
     *                                0xD
     */
    init_gdt_desc(0x0, 0xFFFFF, 0xF3, 0x0D, &kgdt[5]);

    /*
     * User stack segment
     *
     *  base    -> 0x030000:    Beginning of the stack
     *  limit   -> 0x10:        Number of pages for the stack (0x10 * 4 KiB = 64 KiB of stack)
     *  access bits:
     *      Accessed bit:               1
     *      Readable/Writable bit:      1
     *      Direction/Conforming bit:   1
     *      Executable bit:             0
     *
     *      bit 4:                      1
     *      Privilege:                  1
     *                                  1
     *      Present bit:                1
     *                                 --
     *                               0xF7
     * flag bits:
     *      bit 0:                      1
     *      bit 1:                      0
     *      Size bit:                   1
     *      Granularity bit:            1
     *                                 --
     *                                0xD
     */
    init_gdt_desc(0x0, 0x0, 0xF7, 0x0D, &kgdt[6]);

    /*
     * Task state segment
     *
     *  base    -> &default_tss:    Pointer to the default tss
     *  limit   -> 0x67:            Size of the default tss
     *  access bits:
     *      Accessed bit:               1
     *      Readable/Writable bit:      0
     *      Direction/Conforming bit:   0
     *      Executable bit:             1
     *
     *      bit 4:                      0
     *      Privilege:                  1
     *                                  1
     *      Present bit:                1
     *                                 --
     *                               0xE9
     * flag bits:
     *      bit 0:                      0
     *      bit 1:                      0
     *      Size bit:                   0
     *      Granularity bit:            0
     *                                 --
     *                                0xD
     */
    init_gdt_desc((uint32_t) &default_tss, 0x67, 0xE9, 0x00, &kgdt[7]);

    kgdtr.size   = GDT_SIZE * 8;
    kgdtr.offset = GDT_BASE;

    memcpy((char*) kgdtr.offset, (char*) kgdt, kgdtr.size);

    asm("lgdtl (kgdtr)");

    // Set GDT_BASE + third descriptor into segment registers so GDT_BASE+0x10
    // The long jump is directed to the code segment
    asm("movw $0x10, %ax        \n \
         movw %ax, %ds          \n \
         movw %ax, %es          \n \
         movw %ax, %fs          \n \
         movw %ax, %gs          \n \
         ljmp $0x08, $next      \n \
         next:                  \n");
}
