#ifndef _CURSOR_H_
#define _CURSOR_H_

#include <stddef.h>

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
