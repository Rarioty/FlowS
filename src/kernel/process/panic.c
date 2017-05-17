#include <kernel/process/panic.h>

#include <kernel/process/process.h>
#include <kernel/memory/io.h>
#include <stdio.h>

void panic(char* message, const char* file, int line, irq_registers* r)
{
    cli;
    printf("Error: %s\n", message);
    printf("PID: %d\n", current->pid);
    printf("File: %s\n", file);
    printf("Line: %d\n", line);
    if (r)
    {
        printf("Registers dump:\n");
        printf("EAX: %08X  EBX: %08X  ECX: %08X  EDX: %08X\n", r->eax, r->ebx, r->ecx, r->edx);
        printf("ESP: %08X  EBP: %O8X  EDI: %08X  ESI: %08X\n", r->esp, r->ebp, r->edi, r->esi);
        printf("Error code: %d\n", r->err_code);
        print_eflags(r->eflags);
        printf("User ESP: %08X\n", r->useresp);
        printf("EIP: %08X\n", r->eip);
    }

    while(1);
}
