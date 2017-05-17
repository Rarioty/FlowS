#include <kernel/isrs.h>

#include <kernel/utils/registers.h>
#include <kernel/idt.h>
#include <kernel/io.h>
#include <stdio.h>

/*
 * Exception Handlers
 */
extern void _asm_isr_0();
extern void _asm_isr_1();
extern void _asm_isr_2();
extern void _asm_isr_3();
extern void _asm_isr_4();
extern void _asm_isr_5();
extern void _asm_isr_6();
extern void _asm_isr_7();
extern void _asm_isr_8();
extern void _asm_isr_9();
extern void _asm_isr_10();
extern void _asm_isr_11();
extern void _asm_isr_12();
extern void _asm_isr_13();
extern void _asm_isr_14();
extern void _asm_isr_15();
extern void _asm_isr_16();
extern void _asm_isr_17();
extern void _asm_isr_18();
extern void _asm_isr_19();
extern void _asm_isr_20();
extern void _asm_isr_21();
extern void _asm_isr_22();
extern void _asm_isr_23();
extern void _asm_isr_24();
extern void _asm_isr_25();
extern void _asm_isr_26();
extern void _asm_isr_27();
extern void _asm_isr_28();
extern void _asm_isr_29();
extern void _asm_isr_30();
extern void _asm_isr_31();
extern void _asm_isr_48();

char *exception_messages[] = {
	"Division by zero",				/* 0 */
	"Debug",
	"Non-maskable interrupt",
	"Breakpoint",
	"Detected overflow",
	"Out-of-bounds",				/* 5 */
	"Invalid opcode",
	"No coprocessor",
	"Double fault",
	"Coprocessor segment overrun",
	"Bad TSS",						/* 10 */
	"Segment not present",
	"Stack fault",
	"General protection fault",
	"Page fault",
	"Unknown interrupt",			/* 15 */
	"Coprocessor fault",
	"Alignment check",
	"Machine check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

static irq_handler_t isrs_routines[32] = {NULL};

void isrs_install_handler(int irq, irq_handler_t handler)
{
    isrs_routines[irq] = handler;
}

void isrs_uninstall_handler(int irq)
{
    isrs_routines[irq] = NULL;
}

void fault_handler(irq_registers* r)
{
    void (*handler)(irq_registers *r);
    handler = isrs_routines[r->int_no];
    if (handler)
        handler(r);
    else
	{
        printf("Unhandled exception: [%d] %s\n", r->int_no, exception_messages[r->int_no]);
		while (1);
	}
}

void isrs_install(void)
{
    init_idt_desc(0x08, (uint32_t) _asm_isr_0, INTGATE, &kidt[0]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_1, INTGATE, &kidt[1]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_2, INTGATE, &kidt[2]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_3, INTGATE, &kidt[3]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_4, INTGATE, &kidt[4]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_5, INTGATE, &kidt[5]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_6, INTGATE, &kidt[6]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_7, INTGATE, &kidt[7]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_8, INTGATE, &kidt[8]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_9, INTGATE, &kidt[9]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_10, INTGATE, &kidt[10]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_11, INTGATE, &kidt[11]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_12, INTGATE, &kidt[12]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_13, INTGATE, &kidt[13]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_14, INTGATE, &kidt[14]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_15, INTGATE, &kidt[15]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_16, INTGATE, &kidt[16]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_17, INTGATE, &kidt[17]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_18, INTGATE, &kidt[18]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_19, INTGATE, &kidt[19]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_20, INTGATE, &kidt[20]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_21, INTGATE, &kidt[21]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_22, INTGATE, &kidt[22]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_23, INTGATE, &kidt[23]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_24, INTGATE, &kidt[24]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_25, INTGATE, &kidt[25]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_26, INTGATE, &kidt[26]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_27, INTGATE, &kidt[27]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_28, INTGATE, &kidt[28]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_29, INTGATE, &kidt[29]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_30, INTGATE, &kidt[30]);
    init_idt_desc(0x08, (uint32_t) _asm_isr_31, INTGATE, &kidt[31]);

	// Interrupt for syscalls
    init_idt_desc(0x08, (uint32_t) _asm_isr_48, TRAPGATE, &kidt[48]);
}
