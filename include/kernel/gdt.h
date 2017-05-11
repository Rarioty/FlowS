#ifndef _GDT_H_
#define _GDT_H_

/**
 * \file    gdt.h
 * \brief   This file handle the global descriptor table.
 */

#include <kernel/memory_map.h>
#include <stddef.h>

#define GDT_BASE    MEMORY_MAP_GDT_POSITION     /*!<    Physical address of the gdt in memory               */
#define GDT_SIZE    0xFF                        /*!<    Number of reserved slots for segment descriptors    */

/**
 *  \struct     gdtdesc
 *  \brief      Structure of a segment descriptor
 *
 *  A segment descriptor describe the start of the segment in memory, it's size and it's type.<br />
 *  The structure contains:<br />
 *  <ul>
 *      <li>The base of the segment (start)</li>
 *      <li>The limit of the segment (size)</li>
 *      <li>The access byte of the segment</li>
 *      <li>The flags of the segment</li>
 *  </ul>
 *  The access byte have a structure too:
 *  <ul>
 *      <li>bit 0: Accessed bit. Just set to \b 0. The CPU sets this to \b 1 when the segment is accessed.</li>
 *      <li>
 *          bit 1: Redable/Writable bit.
 *          <ul>
 *              <li>Readable bit for code selectors: Whether read access for this segment is allowed. Write access is never allowed for code segments.</li>
 *              <li>Writable bit for data selectors: Whether write access for this segment is allowed. Read access is always allowed for data segments.</li>
 *          </ul>
 *      </li>
 *      <li>
 *          bit 2: Direction bit/Conforming bit.
 *          <ul>
 *              <li>Direction bit for data selectors: Tells the direction. \b 0 the segment grows up. \b 1 the segment grows down, ie. the offset has to be greater than the limit.</li>
 *              <li>
 *                  Conforming bit for code selectors:
 *                  <ul>
 *                      <li>If \b 1 code in this segment can be executed from an equal or lower privilege level. For example, code in ring 3 can far-jump to conforming code in a ring 2 segment. The privl-bits represent the highest privilege level that is allowed to execute the segment. For example, code in ring 0 cannot far-jump to a conforming code segment with privl==0x2, while code in ring 2 and 3 can. Note that the privilege level remains the same, ie. a far-jump form ring 3 to a privl==2-segment remains in ring 3 after the jump.</li>
 *                      <li>if \b 0 code in this segment can only be executed from the ring set in privl.</li>
 *                  </ul>
 *              </li>
 *          </ul>
 *      </li>
 *      <li>bit 3: Executable bit. If \b 1 code in this segment can be executed, ie. a code selector. If \b 0 it is a data selector.</li>
 *      <li>bit 4: Set to \b 1</li>
 *      <li>bit 5-6: Privilege, 2 bits. Contains the ring level, 0 = highest (kernel), 3 = lowest (user applications).</li>
 *      <li>bit 7: Present bit. This must be \b 1 for all valid selectors.</li>
 *  </ul>
 *  The flags have the following bits descriptors:
 *  <ul>
 *      <li>bit 0: Set to \b 0.</li>
 *      <li>bit 1: Set to \b 0.</li>
 *      <li>bit 2: Size bit. If \b 0 the selector defines 16 bit protected mode. If \b 1 it defines 32 bit protected mode. You can have both 16 bit and 32 bit selectors at once.</li>
 *      <li>bit 3: Granularity bit. If \b 0 the limit is in \b 1 B blocks (byte granularity), if \b 1 the limit is in 4 KiB blocks (page granularity).</li>
 *  </ul>
 */
struct gdtdesc
{
    uint16_t lim0_15;       /*!<    Lower bits of the limite                            */
    uint16_t base0_15;      /*!<    Lower bits of the base                              */
    uint8_t base16_23;      /*!<    Mid bits of the base                                */
    uint8_t access;         /*!<    Access bits                                         */
    uint8_t lim16_19:4;     /*!<    High bits of the limite                             */
    uint8_t other:4;        /*!<    Flags                                               */
    uint8_t base24_31;      /*!<    High bits of the base                               */
} __attribute__((packed));

/**
 *  \struct     gdtr
 *  \brief      This structure hold the memory position of the GDT and it's size
 */
struct gdtr
{
    uint16_t size;          /*!<    Number of segment descriptors we can handle         */
    uint32_t offset;        /*!<    Memory position of the GDT                          */
} __attribute__((packed));

/**
 * \brief Initialize and register a segment descriptor
 *
 * \param[in]   base    The base of the segment
 * \param[in]   limite  The size of the segment in page or bytes
 * \param[in]   access  The access bits of the segment
 * \param[in]   other   The flags bits of the segment
 * \param[in]   desc    A pointer to the slot in the GDT where to register the segment descriptor
 */
void init_gdt_desc(uint32_t base, uint32_t limite, uint8_t access, uint8_t other, struct gdtdesc* desc);

/**
 * \brief Initialize the GDT
 */
void init_gdt(void);

#ifdef __KGDT_C_FILE__
    struct gdtdesc kgdt[GDT_SIZE];  /*!<    Slots for the GDT       */
    struct gdtr kgdtr;              /*!<    GDT structure           */
#else
    extern struct gdtdesc kgdt[];   /*!<    Slots for the GDT       */
    extern struct gdtr kgdtr;       /*!<    GDT structure           */
#endif

#endif
