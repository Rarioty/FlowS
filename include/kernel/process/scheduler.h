#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <kernel/utils/registers.h>
#include <kernel/interrupts/idt.h>

/**
 * \file    scheduler.h
 * \brief   This file handle the task scheduler
 */

/**
 * \brief   This function schedule the ready tasks and switch them if needed
 *
 * \param[in] r     Registers in the stack when called
 */
void schedule(irq_registers* r);

#endif
