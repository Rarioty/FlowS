#ifndef _MEMORY_MAP_H_
#define _MEMORY_MAP_H_

/**
 *  \file memory_map.h
 *  \brief This is the memory map of FlowS (from the bottom to the top).
 *
 *  <h1>"Low" memory (< 1MiB)</h1>
 *  <h2>Overview</h2>
 *  \verbatim
 *  Start       End             Size            Type            Usable  Description
 *  -----       ---             ----            ----            ------  -----------
 *  0x00000000  0x000003FF      1 KiB           RAM               0     Real Mode IVT (Interrupt Vector Table)
 *  0x00000400  0x000004FF      256 bytes       RAM               0     BDA (BIOS data area, see below)
 *  0x00000500  0x00007BFF      ~ 30 KiB        RAM               1     Conventional memory used for FlowS (*)      (*): Used for IDT and GDT (see below)
 *  0x00007C00  0x00007DFF      512 bytes       RAM               0     OS BootSector
 *  0x00007E00  0x0007FFFF      480.5 KiB       RAM               1     Conventional memory
 *  0x00080000  0x0009FBFF      ~ 120 KiB (*)   RAM               1     Conventional memory                         (*): Depending on EBDA size (free for use, IF IT EXISTS)
 *  0x0009FC00  0x0009FFFF      1 KiB           RAM               0     EBDA (Extended BIOS Data Area)
 *  0x000A0000  0x000FFFFF      384 KiB         various           0     Video memory, ROM Area
 *  \endverbatim
 *
 *  <h2>FlowS memory area</h2>
 *  \verbatim
 *  Start     End       size                        Description
 *  -----     ---       ----                        -----------
 *  0x0500    0x0CF8    0xFF * 8 bytes (0x7F8)      GDT
 *  0x0D00    0x14F8    0xFF * 8 bytes (0x7F8)      IDT
 *  0x1000    ------    ----                        Kernel page directory
 *  0x20000   0x21000   0x1000                      Page directory
 *  0x21000   0x22000   0x1000                      Page table n°0
 *  ...                 0x1000                      Other page tables
 *  \endverbatim
 *
 * <h2>FlowS upper memory</h2>
 * \verbatim
 *  Start     End       size                        Description
 *  -----     ---       ----                        -----------
 *  0x100000  0x200000  0x100000                    Kernel position
 * \endverbatim
 *
 *  <h2>BIOS Data Area (BDA)</h2>
 *  url: http://www.bioscentral.com/misc/bda.htm
 *  \verbatim
 *  Address     size        Description
 *  -------     ----        -----------
 *
 *  0x0400      4 words     IO ports for COM1-COM4 serial (each address is 1 word, zero if none)
 *  0x0408      3 words     IO ports for LPT1-LPT3 parallel (each address is 1 word, zero if none)
 *  0x040E      1 word      EBDA base address >> 4 (\b usually!)
 *  0x0410      1 word      Packed bit flags for detected hardware
 *  0x0417      1 word      Keyboard state flag
 *  0x041E      32 bytes    Keyboard buffer
 *  0x0449      1 byte      Display mode
 *  0x044A      1 word      Number of columns in text mode
 *  0x0463      2 bytes     Base IO port for video
 *  0x046C      1 word      # of IRQ0 timer ticks since boot
 *  0x0475      1 byte      # of hard disk drives detected
 *  0x0480      1 word      Keyboard buffer start
 *  0x0482      1 word      Keyboard buffer end
 *  0x0497      1 byte      Last keyboard LED/Shift key state
 *  \endverbatim
 *  <h2>ROM Area</h2>
 *  \verbatim
 *  Start       End             Size                    Region/Exception            Description
 *  -----       ---             ----                    ----------------            -----------
 *
 *  0x000A0000  0x000BFFFF      128 KiB                 video RAM                   VGA display memory
 *  0x000C0000  0x000C7FFF      32 KiB (typically)      ROM                         Video BIOS
 *  0x000C8000  0x000EFFFF      160 KiB (typically)     ROMs and unusable space     Mapped hardware & Misc.
 *  0x000F0000  0x000FFFFF      64 KiB                  ROM                         Motherboard BIOS
 *  \endverbatim
 */

/**
 * \brief   Memory position of the kernel page directory
 */
#define KERNEL_PAGE_DIRECTORY_ADDRESS   0x1000

/**
 * \brief   Memory position of the pages for the kernel heap
 */
#define KERNEL_HEAP_PAGE                0x00800000

/**
 * \brief   Memory positition of the end of the pages for the kernel heap
 */
#define KERNEL_HEAP_PAGE_LIMIT          0x10000000

/**
 * \brief   Memory position of the start of the kernel heap
 */
#define KERNEL_HEAP                     0x10000000

/**
 * \brief   Memory position of the end of the kernel heap
 */
#define KERNEL_HEAP_LIMIT               0x40000000

/**
 * \brief   Memory position of the GDT
 *  The GDT has 0xFF segment descriptor and each one is 8 byte long.
 *  The GDT end up in 0x0CF8 !
 */
#define MEMORY_MAP_GDT_POSITION         0x0500

/**
 * \brief   Memory position of the IDT
 *  The IDT has 0xFF interrupt descriptors and each one is 8 byte long.
 *  The IDT end up in 0x14F8
 */
#define MEMORY_MAP_IDT_POSITION         0x0D00

#endif
