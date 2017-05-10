#ifndef _STDIO_H_
#define _STDIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EOF
    #define EOF -1      /*!<    End of file value   */
#endif

/**
 * \brief   Print a formatted string
 *
 * \param[in]   format  Format of the string
 * \param[in]   ...     Arguments for format specification
 *
 * \return  Number of characters written
 */
int printf(const char* __restrict format, ...);

/**
 * \brief   Print a single char
 *
 * \param   c   Character to print
 *
 * \return The character written
 */
int putchar(int c);

/**
 * \brief   Print a string
 *
 * \param   string  The string to print
 *
 * \return The number of characters written
 */
int puts(const char* string);

#ifdef __cplusplus
}
#endif

#endif
