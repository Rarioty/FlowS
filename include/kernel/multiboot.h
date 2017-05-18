#ifndef _MULTIBOOT_H_
#define _MULTIBOOT_H_

#include <stddef.h>

/**
 * \file    multiboot.h
 * \brief   This file handles the multiboot structure and some usefull functions
 */

/**
 * \struct  multiboot_infos
 * \brief   This structure hold the multiboot infos
 *
 *  https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
 */
struct multiboot_infos
{
    uint32_t    flags;              /*!<    Flag of multiboot header, indicate which fields are valid                       */

    /**
     * If bit 0 in the ‘flags’ word is set, then the ‘memory_*’ fields are valid. 'memory_lower' indicates the amount
     * of lower memory in kilobytes. Lower memory starts at address 0 and the maximum possible value for lower memory
     * is 640 kilobytes.
     */
    uint32_t    memory_lower;
    /**
     * If bit 0 in the ‘flags’ word is set, then the ‘memory_*’ fields are valid. memory_upper indicates the amount
     * of upper memory in kilobytes. Upper memory starts at address 1 megabyte. The value returned for upper memory
     * is maximally the address of the first upper memory hole minus 1 megabyte. It is not guaranteed to be this value.
     */
    uint32_t    memory_upper;

    /**
     * If bit 1 in the ‘flags’ word is set, then the ‘boot_device’ field is valid, and indicates which bios disk device the
     * boot loader loaded the OS image from. If the OS image was not loaded from a bios disk, then this field must not be
     * present (bit 3 must be clear). The operating system may use this field as a hint for determining its own root device,
     * but is not required to. The ‘boot_device’ field is laid out in four one-byte subfields as follows:
     * \verbatim
      +-------+-------+-------+-------+
      | part3 | part2 | part1 | drive |
      +-------+-------+-------+-------+ \endverbatim
     *
     * The first byte contains the bios drive number as understood by the bios INT 0x13 low-level disk interface:
     * e.g. 0x00 for the first floppy disk or 0x80 for the first hard disk.
     *
     * The three remaining bytes specify the boot partition. ‘part1’ specifies the top-level partition number,
     * ‘part2’ specifies a sub-partition in the top-level partition, etc. Partition numbers always start from zero.
     * Unused partition bytes must be set to 0xFF. For example, if the disk is partitioned using a simple one-level DOS
     * partitioning scheme, then ‘part1’ contains the DOS partition number, and ‘part2’ and ‘part3’ are both 0xFF.
     * As another example, if a disk is partitioned first into DOS partitions, and then one of those DOS partitions is
     * subdivided into several BSD partitions using BSD's disklabel strategy, then ‘part1’ contains the DOS partition number,
     * ‘part2’ contains the BSD sub-partition within that DOS partition, and ‘part3’ is 0xFF.
     *
     * DOS extended partitions are indicated as partition numbers starting from 4 and increasing, rather than as nested sub-partitions,
     * even though the underlying disk layout of extended partitions is hierarchical in nature. For example, if the boot loader boots
     * from the second extended partition on a disk partitioned in conventional DOS style, then ‘part1’ will be 5, and ‘part2’ and
     * ‘part3’ will both be 0xFF.
     */
    uint32_t    boot_device;

    /**
     * If bit 2 of the ‘flags’ longword is set, the ‘cmdline’ field is valid, and contains the physical address of the command
     * line to be passed to the kernel. The command line is a normal C-style zero-terminated string.
     */
    uint32_t    cmd_line;

    /**
     * If bit 3 of the ‘flags’ is set, then the ‘mods’ fields indicate to the kernel what boot modules were loaded along with the kernel
     * image, and where they can be found. ‘mods_count’ contains the number of modules loaded, it may be zero, indicating no boot modules
     * were loaded, even if bit 1 of ‘flags’ is set.
     */
    uint32_t    mods_count;
    /**
     * If bit 3 of the ‘flags’ is set, then the ‘mods’ fields indicate to the kernel what boot modules were loaded along with the kernel
     * image, and where they can be found. ‘mods_addr’ contains the physical address of the first module structure,
     * Each module structure is formatted as follows:
     *
     \verbatim
              +-------------------+
      0       | mod_start         |
      4       | mod_end           |
              +-------------------+
      8       | string            |
              +-------------------+
      12      | reserved (0)      |
              +-------------------+ \endverbatim
     *
     *
     * The first two fields contain the start and end addresses of the boot module itself. The ‘string’ field provides an arbitrary
     * string to be associated with that particular boot module; it is a zero-terminated ASCII string, just like the kernel command line.
     * The ‘string’ field may be 0 if there is no string associated with the module. Typically the string might be a command
     * line (e.g. if the operating system treats boot modules as executable programs), or a pathname (e.g. if the operating system
     * treats boot modules as files in a file system), but its exact use is specific to the operating system.
     * The ‘reserved’ field must be set to 0 by the boot loader and ignored by the operating system.
     */
    uint32_t    mods_addr;

    /**
     * Caution: Bits 4 & 5 are mutually exclusive.
     *
     * If bit 4 in the ‘flags’ word is set, then the following fields in the Multiboot information structure starting at byte 28 are valid:
     *
     \verbatim
              +-------------------+
      28      | tabsize           |
      32      | strsize           |
      36      | addr              |
      40      | reserved (0)      |
              +-------------------+ \endverbatim
     *
     * These indicate where the symbol table from an a.out kernel image can be found. ‘addr’ is the physical address of the size
     * (4-byte unsigned long) of an array of a.out format nlist structures, followed immediately by the array itself, then the size
     * (4-byte unsigned long) of a set of zero-terminated ascii strings (plus sizeof(unsigned long) in this case), and finally the
     * set of strings itself. ‘tabsize’ is equal to its size parameter (found at the beginning of the symbol section), and ‘strsize’
     * is equal to its size parameter (found at the beginning of the string section) of the following string table to which the symbol
     * table refers. Note that ‘tabsize’ may be 0, indicating no symbols, even if bit 4 in the ‘flags’ word is set.
     *
     * If bit 5 in the ‘flags’ word is set, then the following fields in the Multiboot information structure starting at byte 28 are valid:
     *
     \verbatim
              +-------------------+
      28      | num               |
      32      | size              |
      36      | addr              |
      40      | shndx             |
              +-------------------+ \endverbatim
     *
     * These indicate where the section header table from an ELF kernel is, the size of each entry, number of entries, and the string
     * table used as the index of names. They correspond to the ‘shdr_*’ entries (‘shdr_num’, etc.) in the Executable and Linkable Format
     * (elf) specification in the program header. All sections are loaded, and the physical address fields of the elf section header then
     * refer to where the sections are in memory (refer to the i386 elf documentation for details as to how to read the section header(s)).
     * Note that ‘shdr_num’ may be 0, indicating no symbols, even if bit 5 in the ‘flags’ word is set.
     */
    uint32_t    syms[4];

    /**
     * If bit 6 in the ‘flags’ word is set, then the ‘mmap_*’ fields are valid, and indicate the address and length of a buffer containing
     * a memory map of the machine provided by the bios. ‘mmap_length’ is the total size of the buffer.
     * The buffer consists of one or more of the following size/structure pairs (‘size’ is really used for skipping to the next pair):
     \verbatim
              +-------------------+
      -4      | size              |
              +-------------------+
      0       | base_addr         |
      8       | length            |
      16      | type              |
              +-------------------+ \endverbatim
     *
     * where ‘size’ is the size of the associated structure in bytes, which can be greater than the minimum of 20 bytes. ‘base_addr’
     * is the starting address. ‘length’ is the size of the memory region in bytes. ‘type’ is the variety of address range represented,
     * where a value of 1 indicates available ram, and all other values currently indicated a reserved area.
     *
     * The map provided is guaranteed to list all standard ram that should be available for normal use.
     */
    uint32_t    mmap_length;

    /**
     * If bit 6 in the ‘flags’ word is set, then the ‘mmap_*’ fields are valid, and indicate the address and length of a buffer containing
     * a memory map of the machine provided by the bios. ‘mmap_addr’ is the address.
     * The buffer consists of one or more of the following size/structure pairs (‘size’ is really used for skipping to the next pair):
     \verbatim
              +-------------------+
      -4      | size              |
              +-------------------+
      0       | base_addr         |
      8       | length            |
      16      | type              |
              +-------------------+ \endverbatim
     *
     * where ‘size’ is the size of the associated structure in bytes, which can be greater than the minimum of 20 bytes. ‘base_addr’
     * is the starting address. ‘length’ is the size of the memory region in bytes. ‘type’ is the variety of address range represented,
     * where a value of 1 indicates available ram, and all other values currently indicated a reserved area.
     *
     * The map provided is guaranteed to list all standard ram that should be available for normal use.
     */
    uint32_t    mmap_addr;

    /**
     * If bit 7 in the ‘flags’ is set, then the ‘drives_*’ fields are valid, and indicate the address of the physical address of the first
     * drive structure and the size of drive structures. ‘drives_addr’ is the address, and ‘drives_length’ is the total size of drive structures.
     * Note that ‘drives_length’ may be zero. Each drive structure is formatted as follows:
     \verbatim
             +-------------------+
     0       | size              |
             +-------------------+
     4       | drive_number      |
             +-------------------+
     5       | drive_mode        |
             +-------------------+
     6       | drive_cylinders   |
     8       | drive_heads       |
     9       | drive_sectors     |
             +-------------------+
     10 - xx | drive_ports       |
             +-------------------+ \endverbatim
     *
     * The ‘size’ field specifies the size of this structure. The size varies, depending on the number of ports. Note that the size may not
     * be equal to (10 + 2 * the number of ports), because of an alignment.
     *
     * The ‘drive_number’ field contains the BIOS drive number. The ‘drive_mode’ field represents the access mode used by the boot loader.
     * Currently, the following modes are defined:
     * <ul>
     * <li>‘0’
     *   CHS mode (traditional cylinder/head/sector addressing mode).</li>
     * <li>‘1’
     *   LBA mode (Logical Block Addressing mode).</li>
     * </ul>
     *
     * The three fields, ‘drive_cylinders’, ‘drive_heads’ and ‘drive_sectors’, indicate the geometry of the drive detected by the bios.
     * ‘drive_cylinders’ contains the number of the cylinders. ‘drive_heads’ contains the number of the heads. ‘drive_sectors’ contains
     * the number of the sectors per track.
     *
     * The ‘drive_ports’ field contains the array of the I/O ports used for the drive in the bios code. The array consists of zero or
     * more unsigned two-bytes integers, and is terminated with zero. Note that the array may contain any number of I/O ports that are not
     * related to the drive actually (such as dma controller's ports).
     */
    uint32_t    drives_length;
    /**
     * If bit 7 in the ‘flags’ is set, then the ‘drives_*’ fields are valid, and indicate the address of the physical address of the first
     * drive structure and the size of drive structures. ‘drives_addr’ is the address, and ‘drives_length’ is the total size of drive structures.
     * Note that ‘drives_length’ may be zero. Each drive structure is formatted as follows:
     \verbatim
             +-------------------+
     0       | size              |
             +-------------------+
     4       | drive_number      |
             +-------------------+
     5       | drive_mode        |
             +-------------------+
     6       | drive_cylinders   |
     8       | drive_heads       |
     9       | drive_sectors     |
             +-------------------+
     10 - xx | drive_ports       |
             +-------------------+ \endverbatim
     *
     * The ‘size’ field specifies the size of this structure. The size varies, depending on the number of ports. Note that the size may not
     * be equal to (10 + 2 * the number of ports), because of an alignment.
     *
     * The ‘drive_number’ field contains the BIOS drive number. The ‘drive_mode’ field represents the access mode used by the boot loader.
     * Currently, the following modes are defined:
     * <ul>
     * <li>‘0’
     *   CHS mode (traditional cylinder/head/sector addressing mode).</li>
     * <li>‘1’
     *   LBA mode (Logical Block Addressing mode).</li>
     * </ul>
     *
     * The three fields, ‘drive_cylinders’, ‘drive_heads’ and ‘drive_sectors’, indicate the geometry of the drive detected by the bios.
     * ‘drive_cylinders’ contains the number of the cylinders. ‘drive_heads’ contains the number of the heads. ‘drive_sectors’ contains
     * the number of the sectors per track.
     *
     * The ‘drive_ports’ field contains the array of the I/O ports used for the drive in the bios code. The array consists of zero or
     * more unsigned two-bytes integers, and is terminated with zero. Note that the array may contain any number of I/O ports that are not
     * related to the drive actually (such as dma controller's ports).
     */
    uint32_t    drives_addr;

    /**
     * If bit 8 in the ‘flags’ is set, then the ‘config_table’ field is valid, and indicates the address of the rom configuration table
     * returned by the GET CONFIGURATION bios call. If the bios call fails, then the size of the table must be zero.
     */
    uint32_t    config_table;

    /**
     * If bit 9 in the ‘flags’ is set, the ‘boot_loader_name’ field is valid, and contains the physical address of the name of a boot
     * loader booting the kernel. The name is a normal C-style zero-terminated string.
     */
    uint32_t    boot_loader_name;

    /**
     * If bit 10 in the ‘flags’ is set, the ‘apm_table’ field is valid, and contains the physical address of an apm table defined as below:
     \verbatim
              +----------------------+
      0       | version              |
      2       | cseg                 |
      4       | offset               |
      8       | cseg_16              |
      10      | dseg                 |
      12      | flags                |
      14      | cseg_len             |
      16      | cseg_16_len          |
      18      | dseg_len             |
              +----------------------+ \endverbatim
     *
     * The fields ‘version’, ‘cseg’, ‘offset’, ‘cseg_16’, ‘dseg’, ‘flags’, ‘cseg_len’, ‘cseg_16_len’, ‘dseg_len’ indicate the version number,
     * the protected mode 32-bit code segment, the offset of the entry point, the protected mode 16-bit code segment, the protected mode 16-bit
     * data segment, the flags, the length of the protected mode 32-bit code segment, the length of the protected mode 16-bit code segment,
     * and the length of the protected mode 16-bit data segment, respectively. Only the field ‘offset’ is 4 bytes, and the others are 2 bytes.
     */
    uint32_t    apm_table;

    /**
     * If bit 11 in the ‘flags’ is set, the graphics table is available. This must only be done if the kernel has indicated in the ‘Multiboot Header’
     *  that it accepts a graphics mode.
     *
     * The fields ‘vbe_control_info’ and ‘vbe_mode_info’ contain the physical addresses of vbe control information returned by the vbe Function
     * 00h and vbe mode information returned by the vbe Function 01h, respectively.
     *
     * The field ‘vbe_mode’ indicates current video mode in the format specified in vbe 3.0.
     *
     * The rest fields ‘vbe_interface_seg’, ‘vbe_interface_off’, and ‘vbe_interface_len’ contain the table of a protected mode interface
     * defined in vbe 2.0+. If this information is not available, those fields contain zero. Note that vbe 3.0 defines another protected mode
     * interface which is incompatible with the old one. If you want to use the new protected mode interface, you will have to find the table
     * yourself.
     *
     * The fields for the graphics table are designed for vbe, but Multiboot boot loaders may simulate vbe on non-vbe modes, as if they
     * were vbe modes.
     */
    uint32_t    vbe_control_info;
    /**
     * If bit 11 in the ‘flags’ is set, the graphics table is available. This must only be done if the kernel has indicated in the ‘Multiboot Header’
     *  that it accepts a graphics mode.
     *
     * The fields ‘vbe_control_info’ and ‘vbe_mode_info’ contain the physical addresses of vbe control information returned by the vbe Function
     * 00h and vbe mode information returned by the vbe Function 01h, respectively.
     *
     * The field ‘vbe_mode’ indicates current video mode in the format specified in vbe 3.0.
     *
     * The rest fields ‘vbe_interface_seg’, ‘vbe_interface_off’, and ‘vbe_interface_len’ contain the table of a protected mode interface
     * defined in vbe 2.0+. If this information is not available, those fields contain zero. Note that vbe 3.0 defines another protected mode
     * interface which is incompatible with the old one. If you want to use the new protected mode interface, you will have to find the table
     * yourself.
     *
     * The fields for the graphics table are designed for vbe, but Multiboot boot loaders may simulate vbe on non-vbe modes, as if they
     * were vbe modes.
     */
    uint32_t    vbe_mode_info;
    /**
     * If bit 11 in the ‘flags’ is set, the graphics table is available. This must only be done if the kernel has indicated in the ‘Multiboot Header’
     *  that it accepts a graphics mode.
     *
     * The fields ‘vbe_control_info’ and ‘vbe_mode_info’ contain the physical addresses of vbe control information returned by the vbe Function
     * 00h and vbe mode information returned by the vbe Function 01h, respectively.
     *
     * The field ‘vbe_mode’ indicates current video mode in the format specified in vbe 3.0.
     *
     * The rest fields ‘vbe_interface_seg’, ‘vbe_interface_off’, and ‘vbe_interface_len’ contain the table of a protected mode interface
     * defined in vbe 2.0+. If this information is not available, those fields contain zero. Note that vbe 3.0 defines another protected mode
     * interface which is incompatible with the old one. If you want to use the new protected mode interface, you will have to find the table
     * yourself.
     *
     * The fields for the graphics table are designed for vbe, but Multiboot boot loaders may simulate vbe on non-vbe modes, as if they
     * were vbe modes.
     */
    uint32_t    vbe_mode;
    /**
     * If bit 11 in the ‘flags’ is set, the graphics table is available. This must only be done if the kernel has indicated in the ‘Multiboot Header’
     *  that it accepts a graphics mode.
     *
     * The fields ‘vbe_control_info’ and ‘vbe_mode_info’ contain the physical addresses of vbe control information returned by the vbe Function
     * 00h and vbe mode information returned by the vbe Function 01h, respectively.
     *
     * The field ‘vbe_mode’ indicates current video mode in the format specified in vbe 3.0.
     *
     * The rest fields ‘vbe_interface_seg’, ‘vbe_interface_off’, and ‘vbe_interface_len’ contain the table of a protected mode interface
     * defined in vbe 2.0+. If this information is not available, those fields contain zero. Note that vbe 3.0 defines another protected mode
     * interface which is incompatible with the old one. If you want to use the new protected mode interface, you will have to find the table
     * yourself.
     *
     * The fields for the graphics table are designed for vbe, but Multiboot boot loaders may simulate vbe on non-vbe modes, as if they
     * were vbe modes.
     */
    uint32_t    vbe_interface_seg;
    /**
     * If bit 11 in the ‘flags’ is set, the graphics table is available. This must only be done if the kernel has indicated in the ‘Multiboot Header’
     *  that it accepts a graphics mode.
     *
     * The fields ‘vbe_control_info’ and ‘vbe_mode_info’ contain the physical addresses of vbe control information returned by the vbe Function
     * 00h and vbe mode information returned by the vbe Function 01h, respectively.
     *
     * The field ‘vbe_mode’ indicates current video mode in the format specified in vbe 3.0.
     *
     * The rest fields ‘vbe_interface_seg’, ‘vbe_interface_off’, and ‘vbe_interface_len’ contain the table of a protected mode interface
     * defined in vbe 2.0+. If this information is not available, those fields contain zero. Note that vbe 3.0 defines another protected mode
     * interface which is incompatible with the old one. If you want to use the new protected mode interface, you will have to find the table
     * yourself.
     *
     * The fields for the graphics table are designed for vbe, but Multiboot boot loaders may simulate vbe on non-vbe modes, as if they
     * were vbe modes.
     */
    uint32_t    vbe_interface_off;
    /**
     * If bit 11 in the ‘flags’ is set, the graphics table is available. This must only be done if the kernel has indicated in the ‘Multiboot Header’
     *  that it accepts a graphics mode.
     *
     * The fields ‘vbe_control_info’ and ‘vbe_mode_info’ contain the physical addresses of vbe control information returned by the vbe Function
     * 00h and vbe mode information returned by the vbe Function 01h, respectively.
     *
     * The field ‘vbe_mode’ indicates current video mode in the format specified in vbe 3.0.
     *
     * The rest fields ‘vbe_interface_seg’, ‘vbe_interface_off’, and ‘vbe_interface_len’ contain the table of a protected mode interface
     * defined in vbe 2.0+. If this information is not available, those fields contain zero. Note that vbe 3.0 defines another protected mode
     * interface which is incompatible with the old one. If you want to use the new protected mode interface, you will have to find the table
     * yourself.
     *
     * The fields for the graphics table are designed for vbe, but Multiboot boot loaders may simulate vbe on non-vbe modes, as if they
     * were vbe modes.
     */
    uint32_t    vbe_interface_len;
};

#ifdef __MULTIBOOT__
    struct multiboot_infos* mb_infos = NULL;    /*!<    Saved multiboot informations    */
#else
    extern struct multiboot_infos* mb_infos;    /*!<    Saved multiboot informations    */
#endif

/**
 * \brief   Save the multiboot informations and print them
 *
 * \param[in] infos     Multiboot informations
 */
void load_multiboot_infos(struct multiboot_infos* infos);

/**
 * \brief   Print the multiboot informations in a nice way
 */
void print_multiboot_infos(void);

#endif
