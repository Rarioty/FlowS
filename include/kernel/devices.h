#ifndef _DEVICES_H_
#define _DEVICES_H_

/**
 * \file    devices.h
 * \brief   This file handles the devices installing and initializing functions
 */

/**
 * \brief   Install the keyboard driver
 */
void kbd_install(void);

/**
 * \brief   Install the interval clock driver
 */
void clock_install(void);

#endif
