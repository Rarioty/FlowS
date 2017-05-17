#include <kernel/syscalls.h>

#include <kernel/utils/registers.h>
#include <kernel/isrs.h>
#include <kernel/gdt.h>
#include <kernel/tty.h>
#include <stddef.h>
#include <stdio.h>

static int print(char* s)
{
    return printf("%s", s);
}

static uintptr_t syscalls[] = {
    (uintptr_t)&print,
    0
};
uint32_t num_syscalls;

void syscall_handler(irq_registers* r)
{
    // Check if the syscall number is valid
    if (r->eax >= num_syscalls)
        return;

    uintptr_t location = syscalls[r->eax];

    if (location == 1)
        return;

    uint32_t ret;
    asm volatile("push %1       \n  \
                  push %2       \n  \
                  push %3       \n  \
                  push %4       \n  \
                  push %5       \n  \
                  call *%6      \n  \
                  pop %%ebx     \n  \
                  pop %%ebx     \n  \
                  pop %%ebx     \n  \
                  pop %%ebx     \n  \
                  pop %%ebx     \n"
                  : "=a" (ret) : "r" (r->edi), "r" (r->esi), "r" (r->edx), "r" (r->ecx), "r" (r->ebx), "r" (location));

    r->eax = ret;
}

void syscalls_install(void)
{
    for (num_syscalls = 0; syscalls[num_syscalls] != 0; ++num_syscalls);
    terminal_special("Initializing syscalls\n", TERMINAL_INFO);
    isrs_install_handler(SYSCALL_VECTOR, syscall_handler);
}
