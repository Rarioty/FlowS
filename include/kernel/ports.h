#ifndef _PORTS_H_
#define _PORTS_H_

/****************************************************
 *              VGA             PORTS               *
 * url: http://www.osdever.net/FreeVGA/vga/vga.htm  *
 ***************************************************/

#define PORT_VGA_CRTC_CONTROLLER_ADDRESS_REGISTER       0x3D4
#define PORT_VGA_CRTC_CONTROLLER_DATA_REGISTER          0x3D5

/************************************************************
 *              8042             PS/2                       *
 * url: http://wiki.osdev.org/%228042%22_PS/2_Controller    *
 ***********************************************************/

#define PORT_8042_STATUS_REGISTER   0x64
#define PORT_8042_DATA              0x60
#define PORT_8042_COMMAND_REGISTER  0x64

/****************************************************
 *              8259              PIC               *
 * url: http://wiki.osdev.org/PIC                   *
 ****************************************************/

#define PORT_8259_PIC_MASTER_COMMAND    0x20
#define PORT_8259_PIC_MASTER_DATA       0x21
#define PORT_8259_PIC_SLAVE_COMMAND     0xA0
#define PORT_8259_PIC_SLAVE_DATA        0xA1

#endif
