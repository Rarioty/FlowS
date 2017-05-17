#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

/**
 * \file    syscalls.h
 * \brief   This file handle the syscalls
 */

#define SYSCALL_VECTOR  48      /*!<    Interrupt number for syscall    */

/**
 * \brief   This function initialize and install the syscalls
 */
void syscalls_install(void);

#endif
