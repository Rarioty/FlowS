#ifndef _LIMITS_H_
#define _LIMITS_H_

/**
 * \brief
 */
#define CHAR_BIT	8          /*!<     Number of bits in a `char'.                */

#define SCHAR_MIN	(-128)     /*!<    Minimum value a 'signed char' can hold.     */


#define SCHAR_MAX	127         /*!<    Maximum value a 'signed char' can hold.    */

#define UCHAR_MAX	255         /*!<    Maximum value an `unsigned char' can hold.  (Minimum is 0.) */

#ifdef __CHAR_UNSIGNED__
  #define CHAR_MIN	0           /*!<    Minimum value a `char' can hold.            */
  #define CHAR_MAX	UCHAR_MAX   /*!<    Maximum value a `char' can hold.            */
#else
  #define CHAR_MIN	SCHAR_MIN   /*!<    Minimum value a `char' can hold.            */
  #define CHAR_MAX	SCHAR_MAX   /*!<    Maximum value a `char' can hold.            */
#endif

#define SHRT_MIN	(-32768)    /*!<     Minimum value a `signed short int' can hold.   */
#define SHRT_MAX	32767       /*!<     Maximum value a `signed short int' can hold.   */

#define USHRT_MAX	65535       /*!<     Maximum value an `unsigned short int' can hold.  (Minimum is 0.)   */

#define INT_MIN	(-INT_MAX - 1)    /*!<    Minimum value a `signed int' can hold.    */
#define INT_MAX	2147483647        /*!<    Maximum value a `signed int' can hold.    */

#define UINT_MAX	4294967295U  /*!<    Maximum value an `unsigned int' can hold.  (Minimum is 0.)  */

#if __WORDSIZE == 64
  #define LONG_MAX	9223372036854775807L /*!<    Maximum value a `signed long int' can hold. */
#else
  #define LONG_MAX	2147483647L          /*!<    Maximum value a `signed long int' can hold  */
#endif
#define LONG_MIN	(-LONG_MAX - 1L)     /*!<    Minimum value a `signed long int' can hold  */

#if __WORDSIZE == 64
  #define ULONG_MAX	18446744073709551615UL     /*!<    Maximum value an `unsigned long int' can hold   */
#else
  #define ULONG_MAX	4294967295UL               /*!<    Maximum value an `unsigned long int' can hold   */
#endif

#ifdef __USE_ISOC99

  #define LLONG_MAX	9223372036854775807LL      /*!<     Maximum value a `signed long long int' can hold     */
  #define LLONG_MIN	(-LLONG_MAX - 1LL)         /*!<     Minimum value a `signed long long int' can hold     */

  #define ULLONG_MAX	18446744073709551615ULL    /*!<    Maximum value an `unsigned long long int' can hold  */

#endif /* ISO C99 */

#endif
