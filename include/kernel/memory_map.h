#ifndef _MEMORY_MAP_H_
#define _MEMORY_MAP_H_

/**
 *  \file memory_map.h
 *  \brief This is the memory map of FlowS (from the bottom to the top):
 *
 *  <h1>"Low" memory (< 1MiB)</h1>
 *  <h2>Overview</h2>
 *  \verbatim
 *  Start       End             Size            Type            Usable  Description
 *  -----       ---             ----            ----            ------  -----------
 *
 *  0x00000000  0x000003FF      1 KiB           RAM               0     Real Mode IVT (Interrupt Vector Table)
 *  0x00000400  0x000004FF      256 bytes       RAM               0     BDA (BIOS data area, see below)
 *  0x00000500  0x00007BFF      ~ 30 KiB        RAM               1     Conventional memory
 *  0x00007C00  0x00007DFF      512 bytes       RAM               0     OS BootSector
 *  0x00007E00  0x0007FFFF      480.5 KiB       RAM               1     Conventional memory
 *  0x00080000  0x0009FBFF      ~ 120 KiB (*)   RAM               1     Conventional memory                     (*): Depending on EBDA size (free for use, \b if \b it \b exists)
 *  0x0009FC00  0x0009FFFF      1 KiB           RAM               0     EBDA (Extended BIOS Data Area)
 *  0x000A0000  0x000FFFFF      384 KiB         various           0     Video memory, ROM Area
 *  \endverbatim
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

#endif
