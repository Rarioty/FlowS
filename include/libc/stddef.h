#ifndef _STDDEF_H_
#define _STDDEF_H_

/**
 * \file    stddef.h
 * \brief   This file give new types with specific size.
 */

#define NULL 0

typedef unsigned long   size_t;     /*!<    Size_t can store the maximum size of a theoretically possible object of any type */

typedef unsigned char   uint8_t;    /*<!    8 bits size     */
typedef unsigned short  uint16_t;   /*<!    16 bits size    */
typedef unsigned int    uint32_t;   /*<!    32 bits size    */

#endif
