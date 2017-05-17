#include <kernel/cursor.h>

#include <kernel/memory/ports.h>
#include <kernel/memory/io.h>

void move_cursor(uint8_t x, uint8_t y)
{
    uint16_t pos;

    pos = y * 80 + x;

    outb(PORT_VGA_CRTC_CONTROLLER_ADDRESS_REGISTER, VGA_CURSOR_LOCATION_LOW_REGISTER_COMMAND);
    outb(PORT_VGA_CRTC_CONTROLLER_DATA_REGISTER,    (uint8_t)pos);
    outb(PORT_VGA_CRTC_CONTROLLER_ADDRESS_REGISTER, VGA_CURSOR_LOCATION_HIGH_REGISTER_COMMAND);
    outb(PORT_VGA_CRTC_CONTROLLER_DATA_REGISTER,    (uint8_t)(pos >> 8));
}

void hide_cursor(void)
{
    move_cursor(-1, -1);
}
