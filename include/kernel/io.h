#ifndef _IO_H_
#define _IO_H_

/**
 * \file    io.h
 * \brief   This file handle the input-output functions for the kernel.
 */

#define cli asm("cli"::)    /*!<    Disable the interrupts     */

#define sti asm("sti"::)    /*!<    Enable the interrupts      */

/**
 * \brief Send a value to a port
 *
 * \param[in]   port    Where to send the value
 * \param[in]   value   The value to send
 */
#define outb(port,value)    \
    asm volatile ("outb %%al, %%dx" :: "d" (port), "a" (value));

/**
 * \brief Send a value to a port and make a temporisation
 *
 * \param[in]   port    Where to send the value
 * \param[in]   value   The value to send
 */
#define outbp(port,value)   \
    asm volatile ("outb %%al, %%dx; jmp 1f; 1:" :: "d" (port), "a" (value));

/**
 * \brief Read a value from a port
 *
 * \param[in]   port    Where to read the value
 * \return      The value
 */
#define inb(port) ({                                            \
    unsigned char _v;                                           \
    asm volatile ("inb %%dx, %%al" : "=a"(_v) : "d" (port));    \
    _v;                                                         \
})

#endif
