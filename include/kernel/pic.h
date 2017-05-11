#ifndef _PIC_H_
#define _PIC_H_

/**
 * \file    pic.h
 * \brief   This file handle the pic function.
 */

#define PIC_ICW1_ICW4	      0x01		/*!<    ICW4 (not) needed               */
#define PIC_ICW1_SINGLE	      0x02		/*!<    Single (cascade) mode           */
#define PIC_ICW1_INTERVAL4	  0x04		/*!<    Call address interval 4 (8)     */
#define PIC_ICW1_LEVEL	      0x08		/*!<    Level triggered (edge) mode     */
#define PIC_ICW1_INIT	      0x10		/*!<    Initialization - required!      */

#define PIC_ICW4_8086	      0x01		/*!<    8086/88 (MCS-80/85) mode        */
#define PIC_ICW4_AUTO	      0x02		/*!<    Auto (normal) EOI               */
#define PIC_ICW4_BUF_SLAVE	  0x08		/*!<    Buffered mode/slave             */
#define PIC_ICW4_BUF_MASTER	  0x0C		/*!<    Buffered mode/master            */
#define PIC_ICW4_SFNM	      0x10		/*!<    Special fully nested (not)      */

#define PIC_MASTER_START_OFFSET 0x20    /*!<    Start offset on IDT             */
#define PIC_SLAVE_START_OFFSET  0x70    /*!<    Start offset on IDT             */

#define PIC_MASTER_INTERRUPT_MASK   0x00    /*!<    Interrupt mask of the master    */
#define PIC_SLAVE_INTERRUPT_MASK    0x00    /*!<    Interrupt mask of the slave     */

#define PIC_SLAVE_ID                            0x02    /*!<    ID of the slave                         */
#define PIC_MASTER_PIN_CONNECTION_TO_SLAVE      0x04    /*!<    Mask of slave connection on the pins    */

/**
 * \brief Initialize the PIC
 */
void init_pic(void);

#endif
