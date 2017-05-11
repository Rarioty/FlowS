#ifndef _CURSOR_H_
#define _CURSOR_H_

#include <stddef.h>

/*
 * url: http://www.osdever.net/FreeVGA/vga/crtcreg.htm
 */
#define VGA_CURSOR_LOCATION_LOW_REGISTER_COMMAND       0x0F
#define VGA_CURSOR_LOCATION_HIGH_REGISTER_COMMAND      0x0E

/**
 *  \brief Move the cursor in the \a x, \a y position
 *
 *  \param[in]  x   The x position
 *  \param[in]  y   The y position
 */
void move_cursor(uint8_t x, uint8_t y);

/**
 *  \brief Move the cursor out of sight
 */
void hide_cursor(void);

#endif
