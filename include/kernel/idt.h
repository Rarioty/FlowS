#ifndef _IDT_H_
#define _IDT_H_

/**
 * \file    idt.h
 * \brief   This file handle the interrupt descriptor table.
 */

 #include <kernel/utils/registers.h>
#include <kernel/memory_map.h>
#include <stddef.h>

#define IDT_BASE    MEMORY_MAP_IDT_POSITION         /*!<    Base of the IDT                             */
#define IDT_SIZE    0xFF                            /*!<    Number of slots for interrupts              */

#define INTGATE     0x8E00      /*!<    Bits for gate type for an interrupt gate    */
#define TRAPGATE    0xEF00      /*:<    Bits for gate type for a trap gate          */

/**
 *  \struct     idtdesc
 *  \brief      Structure of an interrupt descriptor
 *
 *  The structure contains:<br />
 *  <ul>
 *      <li>The offset of the interrupt (pointer to the function that handle the ISR)</li>
 *      <li>A code segment selector in the IDT</li>
 *      <li>The flag byte</li>
 *  </ul>
 *  The flag byte have a structure too:
 *  <ul>
 *      <li>bit 0-7: \b 0.</li>
 *      <li>
 *          bit 8-bit 11: Gate type. The possible gate types are:
 *          <ul>
 *              <li>0x5 32 bit task gate</li>
 *              <li>0x6 16 bit interrupt gate</li>
 *              <li>0x7 16 bit trap gate</li>
 *              <li>0xE 32 bit interrupt gate</li>
 *              <li>0xF 32 bit trap gate</li>
 *          </ul>
 *      </li>
 *      <li>bit 12: Storage segment. Set to \b 0 for interrupt gates</li>
 *      <li>bit 13-14: Descriptor Privilege Level.  Gate call protection. Specifies which privilege Level the calling Descriptor minimum should have. So hardware and CPU interrupts can be protected from being called out of userspace.</li>
 *      <li>bit 15: Present. Set to \b 0 for unused interrupts. to make sense this bit ensuring that the idt gate is really an idt gate and not just data</li>
 *  </ul>
 */
struct idtdesc
{
    uint16_t offset0_15;        /*!<    Lower bytes of the offset   */
    uint16_t select;            /*!<    Segment selector            */
    uint16_t type;              /*!<    Flag bits                   */
    uint16_t offset16_31;       /*!<    High bytes of the offset    */
} __attribute__((packed));

/**
 * \struct  idtr
 * \brief   Represent the IDT in memory
 */
struct idtr
{
    uint16_t limite;            /*<!    Number of slots for interurpts  */
    uint32_t base;              /*<!    Position in memory of the IDT   */
} __attribute__((packed));

/**
 * \typedef     irq_handler_t
 * \brief       Type for irq handler function to get access to current state of all registers
 */
typedef void (*irq_handler_t) (irq_registers*);

struct idtr kidtr;              /*!<    IDT of the OS                   */
struct idtdesc kidt[IDT_SIZE];  /*!<    Slots of interrupts             */

/**
 * \brief   Initialize an interrupt and register it
 *
 * \param[in]   select  The code segment selector
 * \param[in]   offset  The pointer to the function that handle the ISR
 * \param[in]   type    The flag for the interrupt
 * \param[in]   desc    The position in the IDT table of this interrupt
 */
void init_idt_desc(uint16_t select, uint32_t offset, uint16_t type, struct idtdesc* desc);

/**
 * \brief   Initialize the IDT
 */
void init_idt(void);

#endif
