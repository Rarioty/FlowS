#include <kernel/tty.h>
#include <stdio.h>

#define __MULTIBOOT__
#include <kernel/multiboot.h>

struct boot_module
{
    uint32_t    mod_start;
    uint32_t    mod_end;
    uint32_t    string;
    uint32_t    _reserved;
};

struct multiboot_memory_map
{
    uint32_t    size;
    uint32_t    base_addr_low, base_addr_high;
    uint32_t    length_low, length_high;
    uint32_t    type;
};

struct multiboot_drive
{
    uint32_t    size;
    uint8_t     drive_number;
    uint8_t     drive_mode;
    uint16_t    drive_cylinders;
    uint8_t     drive_heads;
    uint8_t     drive_sectors;
};

struct multiboot_apm_table
{
    uint16_t    version;
    uint16_t    cseg;
    uint32_t    offset;
    uint16_t    cseg_16;
    uint16_t    dseg;
    uint16_t    flags;
    uint16_t    cseg_len;
    uint16_t    cseg_16_len;
    uint16_t    dseg_len;
};

void load_multiboot_infos(struct multiboot_infos* infos)
{
    mb_infos = infos;

    print_multiboot_infos();
}

void print_multiboot_infos()
{
    uint32_t count;
    int i;

    if (mb_infos == NULL)
    {
        terminal_special("No multiboot infos set, please assign mb_infos structure !\n", TERMINAL_ERROR);
        return;
    }

    for (i = 0; i < 80; ++i)
        terminal_putchar('=');

    printf("Multiboot infos:\n");

    // Upper and lower memory
    if (mb_infos->flags & 0x00000001)
    {
        printf("- Lower memory: 0x00000000 - 0x%08X | Upper memory: 0x00100000 - 0x%08X\n",
            mb_infos->memory_lower * 1024,                  // *1024 because it is in kilobytes
            mb_infos->memory_upper * 1024 + 0x00100000      // Don't forget to add the first 1Mo
        );
    }

    // Boot device
    if (mb_infos->flags & 0x00000002)
    {
        printf("- Boot device:  drive=%02X partition1=%d partition2=%d partition3=%d\n",
            (mb_infos->boot_device & 0xFF),
            (mb_infos->boot_device & 0xFF00) >> 8,
            (mb_infos->boot_device & 0xFF0000) >> 16,
            (mb_infos->boot_device & 0xFF000000) >> 24
        );
    }

    // Command line
    if (mb_infos->flags & 0x00000004)
    {
        printf("- Command line: %s\n", mb_infos->cmd_line);
    }

    // Modules
    if (mb_infos->flags & 0x00000008)
    {
        printf("- Modules count: %d\n", mb_infos->mods_count);
        struct boot_module* module = (struct boot_module*)mb_infos->mods_addr;
        for (count = 0; count < mb_infos->mods_count; ++count)
        {
            printf("  - Module %2d: start=%08X  end=%08X  string='%s'\n", count, module[count].mod_start, module[count].mod_end, module[count].string);
        }
    }

    if (mb_infos->flags & 0x00000010 && mb_infos->flags & 0x00000020)
    {
        terminal_special("Bit 4 & 5 are set in the multiboot header !\n", TERMINAL_ERROR);
        return;
    }

    // Symbols
    if (mb_infos->flags & 0x00000010)
    {
        uint32_t    tabsize = mb_infos->syms[0];
        uint32_t    strsize = mb_infos->syms[1];
        uint32_t    addr    = mb_infos->syms[2];

        printf("- Symbols: addr=%08X  tabsize=%d  strsize=%d\n", addr, tabsize, strsize);
    }

    // Headers
    if (mb_infos->flags & 0x00000020)
    {
        uint32_t    num     = mb_infos->syms[0];
        uint32_t    size    = mb_infos->syms[1];
        uint32_t    addr    = mb_infos->syms[2];
        uint32_t    shndx   = mb_infos->syms[3];

        printf("- Headers: addr=%08X  num=%d  size=%d  shndx=%08X\n", addr, num, size, shndx);
    }

    // Mmap
    if (mb_infos->flags & 0x00000040)
    {
        struct multiboot_memory_map* mmap = (struct multiboot_memory_map*)mb_infos->mmap_addr;

        while ((uint32_t)mmap < mb_infos->mmap_addr + mb_infos->mmap_length)
        {
            printf("- RAM Area: type=%d base=0x%08X%08X length=0x%08X%08X\n",
                mmap->type,
                mmap->base_addr_high, mmap->base_addr_low,
                mmap->length_high, mmap->length_low
            );

            mmap = (struct multiboot_memory_map*)((uint32_t)mmap + mmap->size + sizeof(mmap->size));
        }
    }

    // Drives
    if (mb_infos->flags & 0x00000080)
    {
        struct multiboot_drive* drives = (struct multiboot_drive*)mb_infos->drives_addr;

        while ((uint32_t)drives < mb_infos->drives_addr + mb_infos->drives_length)
        {
            printf("- Drive found: number=%d mode=%d cylinders=%d heads=%d sectors=%d\n",
                drives->drive_number,
                drives->drive_mode,
                drives->drive_cylinders,
                drives->drive_heads,
                drives->drive_sectors
            );

            uint16_t* ports = (uint16_t*)((uint32_t)drives + sizeof(struct multiboot_drive));

            printf("  Ports: ");
            while ((uint32_t)ports < (uint32_t)drives + drives->size)
            {
                printf("%04X ", *ports);
                ports++;
            }
            printf("\n");
            drives = (struct multiboot_drive*) ((uint32_t)drives + drives->size);
        }
    }

    // Config table
    if (mb_infos->flags & 0x00000100)
    {
        printf("- Config table address: %08X\n", mb_infos->config_table);
    }

    // Boot loader name
    if (mb_infos->flags & 0x00000200)
    {
        printf("- Boot loader name: %s\n", mb_infos->boot_loader_name);
    }

    // APM table
    if (mb_infos->flags & 0x00000400)
    {
        struct multiboot_apm_table* table = (struct multiboot_apm_table*)mb_infos->apm_table;

        printf("- APM table found (version %d)\n", table->version);
    }

    // VBE
    if (mb_infos->flags & 0x00000800)
    {
        printf("- Graphic table available\n");
    }

    for (i = 0; i < 80; ++i)
        terminal_putchar('=');
}
