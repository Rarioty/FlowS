%macro DEFINE_IRQ_ENTRY 2
	global _asm_irq_%1
	_asm_irq_%1:
		cli
		push byte 0
		push byte %2
		jmp irq_common_stub
%endmacro

%macro DEFINE_ISR_NOERR 1
	global _asm_isr_%1
	_asm_isr_%1:
		cli
		push byte 0
		push byte %1
		jmp isr_common_stub
%endmacro

%macro DEFINE_ISR_ERR 1
	global _asm_isr_%1
	_asm_isr_%1:
		cli
		push byte %1
		jmp isr_common_stub
%endmacro

extern irq_handler
irq_common_stub:
	cli
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	mov eax, irq_handler
	mov ecx, ebp
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	mov al, 0x20
	out 0x20, al
	sti
	iret

extern fault_handler
isr_common_stub:
	cli
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	mov eax, fault_handler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	mov al, 0x20
	out 0x20, al
	sti
	iret

DEFINE_IRQ_ENTRY	0, 32
DEFINE_IRQ_ENTRY	1, 33
DEFINE_IRQ_ENTRY	2, 34
DEFINE_IRQ_ENTRY	3, 35
DEFINE_IRQ_ENTRY	4, 36
DEFINE_IRQ_ENTRY	5, 37
DEFINE_IRQ_ENTRY	6, 38
DEFINE_IRQ_ENTRY	7, 39
DEFINE_IRQ_ENTRY	8, 40
DEFINE_IRQ_ENTRY	9, 41
DEFINE_IRQ_ENTRY	10, 42
DEFINE_IRQ_ENTRY	11, 43
DEFINE_IRQ_ENTRY	12, 44
DEFINE_IRQ_ENTRY	13, 45
DEFINE_IRQ_ENTRY	14, 46
DEFINE_IRQ_ENTRY	15, 47

DEFINE_ISR_NOERR 0
DEFINE_ISR_NOERR 1
DEFINE_ISR_NOERR 2
DEFINE_ISR_NOERR 3
DEFINE_ISR_NOERR 4
DEFINE_ISR_NOERR 5
DEFINE_ISR_NOERR 6
DEFINE_ISR_NOERR 7
DEFINE_ISR_ERR   8
DEFINE_ISR_NOERR 9
DEFINE_ISR_ERR   10
DEFINE_ISR_ERR   11
DEFINE_ISR_ERR   12
DEFINE_ISR_ERR   13
DEFINE_ISR_ERR   14
DEFINE_ISR_NOERR 15
DEFINE_ISR_NOERR 16
DEFINE_ISR_NOERR 17
DEFINE_ISR_NOERR 18
DEFINE_ISR_NOERR 19
DEFINE_ISR_NOERR 20
DEFINE_ISR_NOERR 21
DEFINE_ISR_NOERR 22
DEFINE_ISR_NOERR 23
DEFINE_ISR_NOERR 24
DEFINE_ISR_NOERR 25
DEFINE_ISR_NOERR 26
DEFINE_ISR_NOERR 27
DEFINE_ISR_NOERR 28
DEFINE_ISR_NOERR 29
DEFINE_ISR_NOERR 30
DEFINE_ISR_NOERR 31
DEFINE_ISR_NOERR 48
