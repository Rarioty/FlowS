#ifndef _ISRS_H_
#define _ISRS_H_

#include <kernel/interrupts/idt.h>

/**
 * \file    isrs.h
 * \brief   This file handle errors interrupts requests and the system call request
 */

/**
 * \brief   Install handler for a specific isr
 */
void isrs_install_handler(int isr, irq_handler_t handler);
/**
 * \brief   Remove the handler of a specific isr
 */
void isrs_uninstall_handler(int isr);

/**
 * \brief   Install all isrs in the IDT but this function does not install any handler !
 */
void isrs_install(void);

#endif
