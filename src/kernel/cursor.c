#include <kernel/cursor.h>
#include <kernel/io.h>

void move_cursor(uint8_t x, uint8_t y)
{
    uint16_t pos;

    pos = y * 80 + x;

    outb(0x3D4, 0x0F);
    outb(0X3D5, (uint8_t)pos);
    outb(0X3D4, 0x0E);
    outb(0x3D5, (uint8_t)(pos >> 8));
}

void hide_cursor(void)
{
    move_cursor(-1, -1);
}
