#ifndef _IRQ_H_
#define _IRQ_H_

#include <kernel/interrupts/idt.h>

/**
 * \file    irq.h
 * \brief   This file handle the sixteen interrupt requests and their handlers
 */

#define CLOCK_IRQ               0
#define KEYBOARD_IRQ            1
#define SERIAL_PORT_1_IRQ       3
#define SERIAL_PORT_2           4
#define LPT2_IRQ                5
#define FLOPPY_IRQ              6
#define PARALLEL_PORT_IRQ       7
#define REAL_TIME_CLOCK_IRQ     8
#define MATH_COPROCESSOR_IRQ    13
#define PRIMARY_DISK_IRQ        14
#define SECONDARY_DISK_IRQ      15

/**
 * \brief   Install a handler for a specific irq
 */
void irq_install_handler(int irq, irq_handler_t handler);
/**
 * \brief   Remove the handler for a specific irq
 */
void irq_uninstall_handler(int irq);

/**
 * \brief   Install all irq in the IDT but this function does not install any handler !
 */
void irq_install(void);

#endif
