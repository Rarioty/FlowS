#ifndef _TTY_H_
#define _TTY_H_

#include <stddef.h>

#define TERMINAL_INFO       1       /*!<    INFO flag to print special string       */
#define TERMINAL_WARNING    2       /*!<    WARNING flag to print special string    */
#define TERMINAL_ERROR      3       /*!<    ERROR flag to print special string      */

/**
 * \brief Initialize the terminal
 */
void terminal_initialize(void);

/**
 * \brief Put a character in the current position in the terminal
 *
 * \param[in]   c   Character to print
 */
void terminal_putchar(char c);

/**
 * \brief Write a string of specified length in the terminal
 *
 * \param[in]   data    The string to print
 * \param[in]   length  The length of the string to print
 */
void terminal_write(const char* data, size_t length);

/**
 * \brief Write a string without specified length in the terminal
 *
 * \param[in]   data    The string to print
 */
void terminal_writestring(const char* data);

/**
 * \brief Write a string followed by EOF in the terminal
 *
 * \param[in]   data    The string to print
 */
void terminal_writeline(const char* data);

/**
 * \brief Print a special string depending on the flag specified
 *
 * \param[in]   data    The string to print
 * \param[in]   flag    The flag for the string
 */
void terminal_special(const char* data, int flag);

#endif
