#ifndef _STRING_H_
#define _STRING_H_

/**
 * \file    string.h
 * \brief   This file handle string and memory functions.
 */

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EOL '\n'        /*!<    End of line character       */

/**
 * \brief   Return the size of a string
 *
 * \param[in]   string  The string
 *
 * \return  The size of the string
 */
size_t strlen(const char* string);

/**
 * \brief   Copy a memory chunk to another
 *
 * \param[in]   dstptr  Destination pointer
 * \param[in]   srcptr  Source pointer
 * \param[in]   size    The size of the chunk to copy
 *
 * \return  The destination pointer
 */
void* memcpy(void* restrict dstptr, const void* srcptr, size_t size);

/**
 * \brief   Compare two chunk of memory
 *
 * \param[in]   aptr    Pointer to the first chunk
 * \param[in]   bptr    Pointer to the second chunk
 * \param[in]   size    Size of the two chunks to compare
 *
 * \return  Result of the comparison
 * \retval  -1  First chunk is lower than the second
 * \retval   1  Second chunk is lower than the first
 * \retval   0  The two chunks are equals
 */
int memcmp(const void* aptr, const void* bptr, size_t size);

/**
 * \brief   Set a whole chunk in memory to a specified value
 *
 * \param[in]   bufptr  The pointer to the chunk
 * \param[in]   value   The value to set
 * \param[in]   size    The size of the chunk
 *
 * \return  The pointer to the chunk
 */
void* memset(void* bufptr, int value, size_t size);

#ifdef __cplusplus
}
#endif

#endif
