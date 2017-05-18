#include <kernel/tty.h>
#include <kernel/vga.h>
#include <string.h>

static const size_t         VGA_WIDTH       = 80;
static const size_t         VGA_HEIGHT      = 25;
static uint16_t* const      VGA_MEMORY      = (uint16_t*) 0xB8000;

static uint8_t base_color  = VGA_COLOR_LIGHT_GREY   | VGA_COLOR_BLACK   << 4;
static uint8_t info_color  = VGA_COLOR_LIGHT_GREEN  | VGA_COLOR_BLACK   << 4;
static uint8_t warn_color  = VGA_COLOR_WHITE        | VGA_COLOR_RED     << 4;
static uint8_t error_color = VGA_COLOR_BLACK        | VGA_COLOR_RED     << 4;

static size_t    terminal_row;
static size_t    terminal_column;
static uint8_t   terminal_color;
static uint16_t* terminal_buffer;

void terminal_initialize(void)
{
    terminal_row    = 0;
    terminal_column = 0;
    terminal_color  = base_color;
    terminal_buffer = VGA_MEMORY;
    for (size_t y = 0; y < VGA_HEIGHT; ++y)
    {
        for (size_t x = 0; x < VGA_WIDTH; ++x)
        {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_scroll()
{
    size_t max = VGA_WIDTH * VGA_HEIGHT - VGA_WIDTH - 1;
    for (size_t index = 0; index < max; ++index)
    {
        terminal_buffer[index] = terminal_buffer[index + VGA_WIDTH];
    }

    for (size_t index = max+1; index < VGA_WIDTH * VGA_HEIGHT-1; ++index)
    {
        terminal_buffer[index] = vga_entry(' ', base_color);
    }
}

void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c)
{
    unsigned char uc = c;

    if (c == '\n')
    {
        terminal_row++;
        terminal_column = 0;
    }
    else if (c == '\r')
    {
        terminal_column = 0;
    }
    else
    {
        terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
        terminal_column++;

        if (terminal_column == VGA_WIDTH)
        {
            terminal_column = 0;
            terminal_row++;
        }
    }

    if (terminal_row == VGA_HEIGHT)
    {
        terminal_scroll();
        terminal_row--;
    }
}

void terminal_write(const char* data, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data)
{
    terminal_write(data, strlen(data));
}

void terminal_writeline(const char* data)
{
    terminal_write(data, strlen(data));
    terminal_write("\r\n", 2);
}

void terminal_special(const char* data, int action)
{
    switch (action)
    {
    case TERMINAL_ERROR:
        terminal_color = error_color;
        terminal_writestring("ERROR: ");
        break;
    case TERMINAL_INFO:
        terminal_color = info_color;
        break;
    case TERMINAL_WARNING:
        terminal_color = warn_color;
        break;
    }
    terminal_write(data, strlen(data));
    terminal_color = base_color;
}
