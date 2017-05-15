#ifndef _ISRS_H_
#define _ISRS_H_

#include <kernel/idt.h>

void isrs_install_handler(int irq, irq_handler_t handler);
void isrs_uninstall_handler(int irq);
void isrs_install(void);

#endif
