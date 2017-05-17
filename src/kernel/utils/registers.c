#include <kernel/utils/registers.h>

#include <kernel/tty.h>
#include <stdio.h>

char* flags[] = {
    "CF ",  // Carry flag
    "",
    "PF ",  // Parity flag
    "",
    "AF ",  // Auxiliary carry flag
    "",
    "ZF ",  // Zero flag
    "SF ",  // Sign flag
    "TF ",  // Trap flag
    "IF ",  // Interrupt enable flag
    "DF ",  // Direction flag
    "OF ",  // Overflow flag
    "",
    "",
    "NT ",  // Nested task flag
    "",
    "RF ",  // Resume flag
    "VM ",  // Virtual 8086 mode flag
    "AC ",  // Alignment check flag (486+)
    "VIF ", // Virtual interrupt flag
    "VIP ", // Virtual interrupt pending flag
    "ID "   // ID flag
};

void print_eflags(uint32_t eflags)
{
    int iopl    = eflags & 0x00003000;  // I/O privilege level
    int i;

    printf("EFLAGS: [ ");
    for (i = 0; i < 22; ++i)
    {
        if (eflags & (1 << i))
            printf("%s", flags[i]);
    }
    printf("] - IOPL: %d\n", iopl);
}
