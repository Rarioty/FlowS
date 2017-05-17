#ifndef _PANIC_H_
#define _PANIC_H_

#include <kernel/utils/registers.h>

/**
 * \brief   This line catch a critical error and wait
 *
 * \param[in] message   Message to display
 * \param[in] file      File where the PANIC macro is called
 * \param[in] line      Line of the file above
 * \param[in] r         Saved registers before the panic
 */
void panic(char* message, const char* file, int line, irq_registers* r);

#define PANIC(message,registers)    panic(message, __FILE__, __LINE__, registers);  /*!<    Macro to trigger a critical state   */

#endif
