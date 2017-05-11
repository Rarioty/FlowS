#ifndef _STDARG_H_
#define _STDARG_H_

/**
 * \file    stdarg.h
 * \brief   This file handle the variable arguments.
 */

#ifndef _VA_LIST
    #define _VA_LIST
    #if defined __GNUC__ && __GNUC__ >= 3
        typedef __builtin_va_list va_list;      /*!<    List of variable argument   */
    #else
        typedef char* va_list;                  /*!<    List of variable argument   */
    #endif
#endif

/**
 * \brief Amount of space required in an argument list (ie. the stack) for an argument of type t.
 *
 * \param[in]   t   Type from which we want the size
 *
 * \return  Amount of space required
 */
#define __va_argsiz(t)	\
	(((sizeof(t) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

/**
 * \brief   Start variable argument list processing by setting AP to point to the argument after pN.
 *
 * \param[in]   ap  The variable argument list
 * \param[in]   pN  The last argument before the variable arguments
 */
#ifdef	__GNUC__
    /*
    * In GNU the stack is not necessarily arranged very neatly in order to
    * pack shorts and such into a smaller argument list. Fortunately a
    * neatly arranged version is available through the use of __builtin_next_arg.
    */
    #define va_start(ap, pN)        \
        ((ap) = ((va_list) __builtin_next_arg(pN)))
#else
    /*
    * For a simple minded compiler this should work (it works in GNU too for
    * vararg lists that don't follow shorts and such).
    */
    #define va_start(ap, pN)        \
        ((ap) = ((va_list) (&pN) + __va_argsiz(pN)))
#endif

/**
 * \brief   End processing of variable argument list. In this case we do nothing.
 *
 * \param[in]   ap  The variable argument list
 */
#define va_end(ap)	((void)0)

/**
 * \brief   Return an argument from the stack with the specified type
 *
 * \param [in]  ap  List of variable arguments
 * \param [in]  t   Type of argument we want
 *
 * \return  The value
 *
 * Increment ap to the next argument in the list while returing a
 * pointer to what ap pointed to first, which is of type t.
 *
 * We cast to void* and then to t* because this avoids a warning about
 * increasing the alignment requirement.
 */
#define va_arg(ap, t)					\
	 (((ap) = (ap) + __va_argsiz(t)),		\
	  *((t*) (void*) ((ap) - __va_argsiz(t))))

#endif
