#ifndef _REGISTERS_H_
#define _REGISTERS_H_

#include <stddef.h>

/**
 * \file    registers.h
 * \brief   This file handles some registers structures and utils function about them
 */

/**
 * \struct  interrupt_saved_registers
 * \brief   Structure to save registers when an irq pop
 */
struct interrupt_saved_registers
{
    uint32_t gs;            /*!<    Extra segment register                                                                                  */
    uint32_t fs;            /*!<    Extra segment register                                                                                  */
    uint32_t es;            /*!<    Extra segment register                                                                                  */
    uint32_t ds;            /*!<    Data segment register                                                                                   */

    uint32_t edi;           /*!<    Destination index register, used for string and memory array copying                                    */
    uint32_t esi;           /*!<    Source index register, used for string and memory array copying                                         */
    uint32_t ebp;           /*!<    Stack base pointer register, holds the base address of the stack                                        */
    uint32_t esp;           /*!<    Stack pointer register, holds the top address of the stack                                              */
    uint32_t ebx;           /*!<    Base register, generally used for base pointer for memory access, gets some interrupts return values    */
    uint32_t edx;           /*!<    Data register, generally used for I/O port access, arithmetic, some interrupts calls                    */
    uint32_t ecx;           /*!<    Counter register, generally used for loop counter and for shifts, gets some interrupts values           */
    uint32_t eax;           /*!<    Accumulator register, used for I/O port access, arithmetic, interrupt calls, etc...                     */

    uint32_t int_no;        /*!<    Number of the irq                                                                                       */
    uint32_t err_code;      /*!<    Error code in case of ISR with an IRQ that generate one (for example GP #13)                            */

    uint32_t eip;           /*!<    Instruction pointer register, holds the offset of the next instruction. It can only be read             */
    uint32_t cs;            /*!<    Code segment register                                                                                   */
    uint32_t eflags;        /*!<    Flags register, holds the flags of the processor                                                        */
    uint32_t useresp;       /*!<    Stack pointer of the user before the irq call                                                           */
    uint32_t ss;            /*!<    Stack segment register                                                                                  */
};

struct process_registers
{
    uint32_t eax;           /*!<    Accumulator register, used for I/O port access, arithmetic, interrupt calls, etc...                     */
    uint32_t ecx;           /*!<    Counter register, generally used for loop counter and for shifts, gets some interrupts values           */
    uint32_t edx;           /*!<    Data register, generally used for I/O port access, arithmetic, some interrupts calls                    */
    uint32_t ebx;           /*!<    Base register, generally used for base pointer for memory access, gets some interrupts return values    */
    uint32_t esp;           /*!<    Stack pointer register, holds the top address of the stack                                              */
    uint32_t ebp;           /*!<    Stack base pointer register, holds the base address of the stack                                        */
    uint32_t esi;           /*!<    Source index register, used for string and memory array copying                                         */
    uint32_t edi;           /*!<    Destination index register, used for string and memory array copying                                    */
    uint32_t eip;           /*!<    Instruction pointer register, holds the offset of the next instruction. It can only be read             */
    uint32_t eflags;        /*!<    Flags register, holds the flags of the processor                                                        */
    uint16_t cs;            /*!<    Code segment register                                                                                   */
    uint16_t ss;            /*!<    Stack segment register                                                                                  */
    uint16_t ds;            /*!<    Data segment register                                                                                   */
    uint16_t es;            /*!<    Extra segment register                                                                                  */
    uint16_t fs;            /*!<    Extra segment register                                                                                  */
    uint16_t gs;            /*!<    Extra segment register                                                                                  */
    uint32_t cr3;           /*!<    Control register 3, used with virtual addressing, contains page directory address                       */
} __attribute__((packed));

typedef struct interrupt_saved_registers    irq_registers;
typedef struct process_registers            process_registers;

/**
 * \brief   Print the content of EFLAGS
 *
 * \param[in] eflags    EFLAGS register
 */
void print_eflags(uint32_t eflags);

#endif
