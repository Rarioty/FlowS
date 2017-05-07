#ifndef _TTY_H_
#define _TTY_H_

#include <stddef.h>

void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t length);
void terminal_writestring(const char* data);
void terminal_writeline(const char* data);
void terminal_error(const char* data);

#endif
